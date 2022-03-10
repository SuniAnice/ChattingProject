


#include "NetworkManager.h"
#include <locale.h>


std::wstring mbs_to_wcs( std::string str )
{
	const char* strs = str.c_str();
	wchar_t wcs[ BUFFER_SIZE ] = { 0 };
	mbstate_t shiftState = mbstate_t();
	setlocale( LC_ALL, "" );
	mbsrtowcs( wcs, &strs, sizeof( wcs ), &shiftState );
	std::wstring ret( wcs );
	return ret;
}

// Sets default values
ANetworkManager::ANetworkManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANetworkManager::BeginPlay()
{
	Super::BeginPlay();
	m_instance = Cast< UChattingInstance >( GetWorld()->GetGameInstance() );
}

// Called every frame
void ANetworkManager::Tick(float DeltaTime)
{
	Super::Tick( DeltaTime );
	if ( !m_isServerOff )
	{
		Recv();
	}
	else
	{
		QuitClient();
	}
	

}

int ANetworkManager::Send( std::string& buffer, int32 size )
{
	int32 byte;
	bool ret;
	m_sends.push( buffer );
	// 큐에 미전송 데이터가 있으면 전송
	while ( m_sends.size() != 0 )
	{
		ret = m_instance->m_serverSocket->Send( ( uint8* )m_sends.front().c_str(), m_sends.front().size(), byte );
		// 전송이 중간에 잘렸으면 큐에 잘린 부분을 넣고 다음에 마저 전송한다.
		if ( byte != m_sends.front().size() )
		{
			m_sends.front() = m_sends.front().substr( byte );
			return byte;
		}
		m_sends.pop();
	}
	return byte;
}

int ANetworkManager::Recv()
{
	int32 byte;
	// 남은 덩어리가 있다면 버퍼에 붙임
	if ( m_leftovers.size() != 0 )
	{
		memcpy_s( m_buffer, BUFFER_SIZE, m_leftovers.c_str(), m_leftovers.size() );
		m_recvBytes += m_leftovers.size();
		m_leftovers.clear();
	}
	bool ret = m_instance->m_serverSocket->Recv( m_buffer + m_recvBytes, BUFFER_SIZE - m_recvBytes, byte );
	m_recvBytes += byte;
	if ( !ret )
	{
		m_isServerOff = true;
	}
	else if ( byte != 0 )
	{
		ProcessPacket();
	}
	return byte;
}

void ANetworkManager::InitializeBuffer()
{
	memset( &m_buffer, 0, BUFFER_SIZE );
	m_recvBytes = 0;
}

void ANetworkManager::ProcessPacket() 
{
	std::string str = (char*)m_buffer;
	// 패킷 쪼개기
	char* prev = (char*)m_buffer;
	char* ptr = strstr( prev, "\n\r" );
	while ( ptr != NULL )
	{
		m_packets.push( str.substr( prev - (char*)m_buffer, ptr - prev + 2 ) );
		prev = ptr + 2;
		ptr = strstr( ptr + 2, "\n\r" );
	}
	// 처리하고 남은 덩어리 저장
	m_leftovers = str.substr( prev - (char*)m_buffer );

	std::wstring wstr;

	TArray < FString > arr;


	while ( m_packets.size() != 0 )
	{
		std::string current = m_packets.front();
		const char* cprev = current.c_str();

		// 플레이어 목록의 경우
		const char* p = strstr( current.c_str(), "접속중인 플레이어들을 출력합니다.\r\n" );
		if ( p != NULL )
		{
			// 줄 단위 파싱
			TArray < FString > tarr;
			// 첫 줄 무시
			p = strstr( current.c_str(), "\r\n" );
			cprev = p + 2;
			p = strstr( cprev, "\r\n" );
			while ( p != NULL )
			{
				tarr.Push( std::move( mbs_to_wcs( current.substr( cprev - current.c_str(), p - cprev ) ) ).c_str() );
				cprev = p + 2;
				p = strstr( p + 2, "\r\n");
			}
			PrintUserList( tarr );
			m_packets.pop();
			continue;
		}

		// 방 목록의 경우
		p = strstr( current.c_str(), "방 목록을 출력합니다.\r\n" );
		if ( p != NULL )
		{
			// 줄 단위 파싱
			TArray < FString > tarr;
			// 첫 줄 무시
			p = strstr( current.c_str(), "\r\n" );
			cprev = p + 2;
			p = strstr( cprev, "\r\n" );
			while ( p != NULL )
			{
				tarr.Push( std::move( mbs_to_wcs( current.substr( cprev - current.c_str(), p - cprev ) ) ).c_str() );
				cprev = p + 2;
				p = strstr( p + 2, "\r\n" );
			}
			PrintRoomList( tarr );
			m_packets.pop();
			continue;
		}

		// 방 입장의 경우
		p = strstr( current.c_str(), "채팅방에 입장했습니다. :	" );
		if ( p != NULL )
		{
			current.erase( current.size() - 3, 3 );
			EnterRoom( std::move( mbs_to_wcs( current ).c_str() ) );
			m_packets.pop();
			continue;
		}

		// 방 퇴장의 경우
		p = strstr( current.c_str(), "채팅방에서 나갔습니다.\r\n" );
		if ( p != NULL )
		{
			ExitRoom();
			current.erase( current.size() - 3, 3 );
			arr.Push( std::move( mbs_to_wcs( current ).c_str() ) );
			m_packets.pop();
			continue;
		}

		// 닉네임 생성의 경우
		p = strstr( current.c_str(), "을 닉네임으로 사용합니다.\r\n" );
		if ( p != NULL )
		{
			// 닉네임을 잘라서 함수에 전달
			m_nickname = current.substr( 0, p - current.c_str() );
			LoginSuccess( std::move( mbs_to_wcs( m_nickname ).c_str() ) );
			current.erase( current.size() - 3, 3 );
			arr.Push( std::move( mbs_to_wcs( current ).c_str() ) );
			m_packets.pop();
			continue;
		}

		// 닉네임이 이미 생성되었고, 수신한 메시지가 내가 말하는 메시지일 경우
		if ( m_nickname.size() != 0 )
		{
			// 나의 채팅이면
			p = strstr( current.c_str(), ( m_nickname + " : " ).c_str() );
			if ( p != NULL )
			{
				// 앞의 닉네임부 제거
				std::string temp = current.substr( m_nickname.size() + 3 );
				temp.erase( temp.size() - 3, 3 );
				PrintMyChat( std::move( mbs_to_wcs( temp ).c_str() ) );
				m_packets.pop();
				continue;
			}
			// 나의 귓속말이면
			p = strstr( current.c_str(), "님에게 귓속말 : " );
			if ( p != NULL )
			{
				current.erase( current.size() - 3, 3 );
				PrintMyChat( std::move( mbs_to_wcs( current ).c_str() ) );
				m_packets.pop();
				continue;
			}
		}

		// 마지막 줄 줄나눔 제거
		current.erase( current.size() - 3, 3 );
		arr.Push( std::move( mbs_to_wcs( current ).c_str() ) );
		m_packets.pop();
	}

	if ( arr.Num() != 0 )	PrintBuffers( arr );

	InitializeBuffer();
}
