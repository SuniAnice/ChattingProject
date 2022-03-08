


#include "NetworkManager.h"
#include <locale.h>


std::wstring mbs_to_wcs( std::string str )
{
	const char* strs = str.c_str();
	wchar_t wcs[ BUFFER_SIZE ] = { 0 };
	mbstate_t shiftState = mbstate_t();
	setlocale( LC_ALL, "" );
	mbsrtowcs( wcs, &strs, sizeof( wcs ), &shiftState );
	return std::wstring( wcs );
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

int ANetworkManager::Send( const void* buffer, int32 size )
{
	int32 byte;
	bool ret = m_instance->m_serverSocket->Send( ( uint8* )buffer, size, byte );
	return byte;
}

int ANetworkManager::Recv()
{
	int32 byte;
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

	char* prev = (char*)m_buffer;
	char* ptr = strstr( prev, "\n\r" );
	while ( ptr != NULL )
	{
		m_packets.push( str.substr( prev - (char*)m_buffer, ptr - prev + 1 ) );
		prev = ptr + 1;
		ptr = strstr( ptr + 1, "\n\r" );
	}

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
				tarr.Push( mbs_to_wcs( current.substr( cprev - current.c_str(), p - cprev ) ).c_str() );
				cprev = p + 2;
				p = strstr( p + 2, "\r\n");
			}
			PrintRoomList( tarr );
			m_packets.pop();
			continue;
		}

		// 마지막 줄 줄나눔 제거
		current.pop_back();
		current.pop_back();
		arr.Push( mbs_to_wcs( current ).c_str() );
		m_packets.pop();
	}

	if ( arr.Num() != 0 )	PrintBuffers( arr );

	InitializeBuffer();
}
