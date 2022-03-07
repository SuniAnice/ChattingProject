


#include "NetworkManager.h"
#include <string>
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
	if ( Recv() != 0 )
	{
		std::string str = (char*)m_buffer;
		std::wstring wstr = mbs_to_wcs( str );
		PrintBuffer( wstr.c_str() );
	}
}

int ANetworkManager::Send( const void* buffer, int32 size )
{
	int32 byte;
	m_instance->m_serverSocket->Send( ( uint8* )buffer, size, byte );
	return byte;
}

int ANetworkManager::Recv()
{
	int32 byte;
	m_instance->m_serverSocket->Recv( m_buffer, BUFFER_SIZE, byte );
	return byte;
}
