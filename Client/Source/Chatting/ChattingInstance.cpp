// Fill out your copyright notice in the Description page of Project Settings.


#include "ChattingInstance.h"


constexpr int32 SERVER_PORT = 4000;


UChattingInstance::UChattingInstance() {}

UChattingInstance::~UChattingInstance()
{
    m_serverSocket->Close();
}

void UChattingInstance::Init()
{
    Super::Init();
    m_serverSocket = ISocketSubsystem::Get( PLATFORM_SOCKETSUBSYSTEM )->CreateSocket( NAME_Stream, TEXT( "default" ), false );

    FString address = TEXT( "127.0.0.1" );
    int32 port = SERVER_PORT;
    FIPv4Address ip;
    FIPv4Address::Parse( address, ip );

    TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get( PLATFORM_SOCKETSUBSYSTEM )->CreateInternetAddr();
    addr->SetIp( ip.Value );
    addr->SetPort( SERVER_PORT );

    m_serverSocket->Connect( *addr );
    m_serverSocket->SetNonBlocking();
}

int UChattingInstance::Send( std::string& buffer, int32 size )
{
	int32 byte;
	bool ret;
	m_sends.push( buffer );
	// ť�� ������ �����Ͱ� ������ ����
	while ( m_sends.size() != 0 )
	{
		ret = m_serverSocket->Send( (uint8*)m_sends.front().c_str(), m_sends.front().size(), byte );
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

int UChattingInstance::Recv()
{
	int32 byte;
	// ���� ����� �ִٸ� ���ۿ� ����
	if ( m_leftovers.size() != 0 )
	{
		memcpy_s( m_buffer, BUFFER_SIZE, m_leftovers.c_str(), m_leftovers.size() );
		m_recvBytes += m_leftovers.size();
		m_leftovers.clear();
	}
	bool ret = m_serverSocket->Recv( m_buffer + m_recvBytes, BUFFER_SIZE - m_recvBytes, byte );
	m_recvBytes += byte;
	if ( !ret )
	{
		m_isServerOff = true;
	}
	if ( byte != 0 )
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
	}
	return byte;
}

void UChattingInstance::InitializeBuffer()
{
	memset( &m_buffer, 0, BUFFER_SIZE );
	m_recvBytes = 0;
}
