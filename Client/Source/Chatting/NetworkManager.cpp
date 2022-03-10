


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
	// ť�� ������ �����Ͱ� ������ ����
	while ( m_sends.size() != 0 )
	{
		ret = m_instance->m_serverSocket->Send( ( uint8* )m_sends.front().c_str(), m_sends.front().size(), byte );
		// ������ �߰��� �߷����� ť�� �߸� �κ��� �ְ� ������ ���� �����Ѵ�.
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
	// ���� ����� �ִٸ� ���ۿ� ����
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
	// ��Ŷ �ɰ���
	char* prev = (char*)m_buffer;
	char* ptr = strstr( prev, "\n\r" );
	while ( ptr != NULL )
	{
		m_packets.push( str.substr( prev - (char*)m_buffer, ptr - prev + 2 ) );
		prev = ptr + 2;
		ptr = strstr( ptr + 2, "\n\r" );
	}
	// ó���ϰ� ���� ��� ����
	m_leftovers = str.substr( prev - (char*)m_buffer );

	std::wstring wstr;

	TArray < FString > arr;


	while ( m_packets.size() != 0 )
	{
		std::string current = m_packets.front();
		const char* cprev = current.c_str();

		// �÷��̾� ����� ���
		const char* p = strstr( current.c_str(), "�������� �÷��̾���� ����մϴ�.\r\n" );
		if ( p != NULL )
		{
			// �� ���� �Ľ�
			TArray < FString > tarr;
			// ù �� ����
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

		// �� ����� ���
		p = strstr( current.c_str(), "�� ����� ����մϴ�.\r\n" );
		if ( p != NULL )
		{
			// �� ���� �Ľ�
			TArray < FString > tarr;
			// ù �� ����
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

		// �� ������ ���
		p = strstr( current.c_str(), "ä�ù濡 �����߽��ϴ�. :	" );
		if ( p != NULL )
		{
			current.erase( current.size() - 3, 3 );
			EnterRoom( std::move( mbs_to_wcs( current ).c_str() ) );
			m_packets.pop();
			continue;
		}

		// �� ������ ���
		p = strstr( current.c_str(), "ä�ù濡�� �������ϴ�.\r\n" );
		if ( p != NULL )
		{
			ExitRoom();
			current.erase( current.size() - 3, 3 );
			arr.Push( std::move( mbs_to_wcs( current ).c_str() ) );
			m_packets.pop();
			continue;
		}

		// �г��� ������ ���
		p = strstr( current.c_str(), "�� �г������� ����մϴ�.\r\n" );
		if ( p != NULL )
		{
			// �г����� �߶� �Լ��� ����
			m_nickname = current.substr( 0, p - current.c_str() );
			LoginSuccess( std::move( mbs_to_wcs( m_nickname ).c_str() ) );
			current.erase( current.size() - 3, 3 );
			arr.Push( std::move( mbs_to_wcs( current ).c_str() ) );
			m_packets.pop();
			continue;
		}

		// �г����� �̹� �����Ǿ���, ������ �޽����� ���� ���ϴ� �޽����� ���
		if ( m_nickname.size() != 0 )
		{
			// ���� ä���̸�
			p = strstr( current.c_str(), ( m_nickname + " : " ).c_str() );
			if ( p != NULL )
			{
				// ���� �г��Ӻ� ����
				std::string temp = current.substr( m_nickname.size() + 3 );
				temp.erase( temp.size() - 3, 3 );
				PrintMyChat( std::move( mbs_to_wcs( temp ).c_str() ) );
				m_packets.pop();
				continue;
			}
			// ���� �ӼӸ��̸�
			p = strstr( current.c_str(), "�Կ��� �ӼӸ� : " );
			if ( p != NULL )
			{
				current.erase( current.size() - 3, 3 );
				PrintMyChat( std::move( mbs_to_wcs( current ).c_str() ) );
				m_packets.pop();
				continue;
			}
		}

		// ������ �� �ٳ��� ����
		current.erase( current.size() - 3, 3 );
		arr.Push( std::move( mbs_to_wcs( current ).c_str() ) );
		m_packets.pop();
	}

	if ( arr.Num() != 0 )	PrintBuffers( arr );

	InitializeBuffer();
}
