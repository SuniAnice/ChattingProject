// Fill out your copyright notice in the Description page of Project Settings.


#include "ChattingInstance.h"


constexpr int32 SERVER_PORT = 4000;


UChattingInstance::UChattingInstance()
{
    m_serverSocket = ISocketSubsystem::Get( PLATFORM_SOCKETSUBSYSTEM )->CreateSocket( NAME_Stream, TEXT( "default" ), false );

    FString address = TEXT( "127.0.0.1" );
    int32 port = SERVER_PORT;
    FIPv4Address ip;
    FIPv4Address::Parse( address, ip );

    TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get( PLATFORM_SOCKETSUBSYSTEM )->CreateInternetAddr();
    addr->SetIp( ip.Value );
    addr->SetPort( SERVER_PORT );

    m_serverSocket->Connect(*addr);
    m_serverSocket->SetNonBlocking();
}

UChattingInstance::~UChattingInstance()
{
    m_serverSocket->Close();
}
