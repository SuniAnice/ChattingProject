


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
			m_instance->m_packets.pop();
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
			m_instance->m_packets.pop();
			continue;
		}

		// �� ������ ���
		p = strstr( current.c_str(), "ä�ù濡 �����߽��ϴ�. :	" );
		if ( p != NULL )
		{
			current.erase( current.size() - 3, 3 );
			EnterRoom( std::move( mbs_to_wcs( current ).c_str() ) );
			m_instance->m_packets.pop();
			continue;
		}

		// �� ������ ���
		p = strstr( current.c_str(), "ä�ù濡�� �������ϴ�.\r\n" );
		if ( p != NULL )
		{
			ExitRoom();
			current.erase( current.size() - 3, 3 );
			arr.Push( std::move( mbs_to_wcs( current ).c_str() ) );
			m_instance->m_packets.pop();
			continue;
		}

		// �г��� ������ ���
		p = strstr( current.c_str(), "�� �г������� ����մϴ�.\r\n" );
		if ( p != NULL )
		{
			// �г����� �߶� �Լ��� ����
			m_instance->m_nickname = current.substr( 0, p - current.c_str() );
			LoginSuccess( std::move( mbs_to_wcs( m_instance->m_nickname ).c_str() ) );
			current.erase( current.size() - 3, 3 );
			arr.Push( std::move( mbs_to_wcs( current ).c_str() ) );
			m_instance->m_packets.pop();
			continue;
		}

		// �ʴ��� ���
		p = strstr( current.c_str(), "�Բ��� " );
		const char* pt = strstr( current.c_str(), "�� ä�ù����� �ʴ��ϼ̽��ϴ�.\r\n" );
		if ( p != NULL && pt != NULL )
		{
			// �г��Ӻ� �ڸ���
			auto s1 = current.substr( 0, p - current.c_str() );
			// ���ȣ �ڸ���
			auto s2 = current.substr( p - current.c_str() + 7, pt - p - 7 );
			RecvInvite( mbs_to_wcs( s1 ).c_str(), std::stoi( s2 ) );
			m_instance->m_packets.pop();
			continue;
		}

		// �г����� �̹� �����Ǿ���, ������ �޽����� ���� ���ϴ� �޽����� ���
		if ( m_instance->m_nickname.size() != 0 )
		{
			// ���� ä���̸�
			p = strstr( current.c_str(), ( m_instance->m_nickname + " : " ).c_str() );
			if ( p != NULL )
			{
				// ���� �г��Ӻ� ����
				std::string temp = current.substr( m_instance->m_nickname.size() + 3 );
				temp.erase( temp.size() - 3, 3 );
				PrintMyChat( std::move( mbs_to_wcs( temp ).c_str() ) );
				m_instance->m_packets.pop();
				continue;
			}
			// ���� �ӼӸ��̸�
			p = strstr( current.c_str(), "�Կ��� �ӼӸ� : " );
			if ( p != NULL )
			{
				current.erase( current.size() - 3, 3 );
				PrintMyChat( std::move( mbs_to_wcs( current ).c_str() ) );
				m_instance->m_packets.pop();
				continue;
			}
		}

		// ������ �� �ٳ��� ����
		current.erase( current.size() - 3, 3 );
		arr.Push( std::move( mbs_to_wcs( current ).c_str() ) );
		m_instance->m_packets.pop();
	}

	if ( arr.Num() != 0 )	PrintBuffers( arr );

	m_instance->InitializeBuffer();
}
