


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
	if ( m_instance == nullptr )		QuitClient();
}

// Called every frame
void ANetworkManager::Tick(float DeltaTime)
{
	Super::Tick( DeltaTime );
	if ( !m_instance->m_isServerOff )
	{
		DoRecv();
	}
	else
	{
		QuitClient();
	}
}

int ANetworkManager::DoRecv()
{
	int32 byte;
	byte = m_instance->Recv();
	if ( byte != 0 )
	{
		ProcessPacket();
	}
	return byte;
}

void ANetworkManager::ProcessPacket() 
{
	std::wstring wstr;
	TArray < FString > arr;

	while ( m_instance->m_packets.size() != 0 )
	{
		std::string current = m_instance->m_packets.front();
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
			m_instance->m_packets.pop();
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
			m_instance->m_packets.pop();
			continue;
		}

		// 방 입장의 경우
		p = strstr( current.c_str(), "채팅방에 입장했습니다. :	" );
		if ( p != NULL )
		{
			current.erase( current.size() - 3, 3 );
			EnterRoom( std::move( mbs_to_wcs( current ).c_str() ) );
			m_instance->m_packets.pop();
			continue;
		}

		// 방 퇴장의 경우
		p = strstr( current.c_str(), "채팅방에서 나갔습니다.\r\n" );
		if ( p != NULL )
		{
			ExitRoom();
			current.erase( current.size() - 3, 3 );
			arr.Push( std::move( mbs_to_wcs( current ).c_str() ) );
			m_instance->m_packets.pop();
			continue;
		}

		// 닉네임 생성의 경우
		p = strstr( current.c_str(), "을 닉네임으로 사용합니다.\r\n" );
		if ( p != NULL )
		{
			// 닉네임을 잘라서 함수에 전달
			m_instance->m_nickname = current.substr( 0, p - current.c_str() );
			LoginSuccess( std::move( mbs_to_wcs( m_instance->m_nickname ).c_str() ) );
			current.erase( current.size() - 3, 3 );
			arr.Push( std::move( mbs_to_wcs( current ).c_str() ) );
			m_instance->m_packets.pop();
			continue;
		}

		// 초대의 경우
		p = strstr( current.c_str(), "님께서 " );
		const char* pt = strstr( current.c_str(), "번 채팅방으로 초대하셨습니다.\r\n" );
		if ( p != NULL && pt != NULL )
		{
			// 닉네임부 자르기
			auto s1 = current.substr( 0, p - current.c_str() );
			// 방번호 자르기
			auto s2 = current.substr( p - current.c_str() + 7, pt - p - 7 );
			RecvInvite( mbs_to_wcs( s1 ).c_str(), std::stoi( s2 ) );
			m_instance->m_packets.pop();
			continue;
		}

		// 닉네임이 이미 생성되었고, 수신한 메시지가 내가 말하는 메시지일 경우
		if ( m_instance->m_nickname.size() != 0 )
		{
			// 나의 채팅이면
			p = strstr( current.c_str(), ( m_instance->m_nickname + " : " ).c_str() );
			if ( p != NULL )
			{
				// 앞의 닉네임부 제거
				std::string temp = current.substr( m_instance->m_nickname.size() + 3 );
				temp.erase( temp.size() - 3, 3 );
				PrintMyChat( std::move( mbs_to_wcs( temp ).c_str() ) );
				m_instance->m_packets.pop();
				continue;
			}
			// 나의 귓속말이면
			p = strstr( current.c_str(), "님에게 귓속말 : " );
			if ( p != NULL )
			{
				current.erase( current.size() - 3, 3 );
				PrintMyChat( std::move( mbs_to_wcs( current ).c_str() ) );
				m_instance->m_packets.pop();
				continue;
			}
		}

		// 마지막 줄 줄나눔 제거
		current.erase( current.size() - 3, 3 );
		arr.Push( std::move( mbs_to_wcs( current ).c_str() ) );
		m_instance->m_packets.pop();
	}

	if ( arr.Num() != 0 )	PrintBuffers( arr );

	m_instance->InitializeBuffer();
}
