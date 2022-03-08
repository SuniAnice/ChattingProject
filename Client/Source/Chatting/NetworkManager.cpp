


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
		std::string str = (char*)m_buffer;

		char* prev = (char*)m_buffer;
		char* ptr = strstr( prev, "\n" );

		std::wstring wstr;

		TArray < FString > arr;

		// 엔터키가 입력되었을 경우
		while ( ptr != NULL )
		{
			m_packets.push( str.substr( prev - (char*)m_buffer, ptr - prev + 1 ) );
			prev = ptr + 1;
			ptr = strstr( ptr + 1, "\n" );
		}
		while ( m_packets.size() != 0)
		{
			// 멀티바이트에서 유니코드로 변환
			if ( m_packets.size() == 1 )
			{
				// 마지막 줄 줄나눔 제거
				m_packets.front().pop_back();
				m_packets.front().pop_back();
			}
			arr.Push( mbs_to_wcs( m_packets.front() ).c_str() );
			m_packets.pop();
		}
		PrintBuffers( arr );
		InitializeBuffer();
	}
	return byte;
}

void ANetworkManager::InitializeBuffer()
{
	memset( &m_buffer, 0, BUFFER_SIZE );
	m_recvBytes = 0;
}
