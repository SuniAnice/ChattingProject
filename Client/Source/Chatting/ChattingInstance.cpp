// Fill out your copyright notice in the Description page of Project Settings.


#include "ChattingInstance.h"


constexpr int SERVER_PORT = 4000;


UChattingInstance::UChattingInstance()
{
    WSAStartup( MAKEWORD( 2, 2 ), &wsaData );

    serverSocket = WSASocket( AF_INET, SOCK_STREAM, 0, NULL, 0, 0 );

    memset( &serverAddr, 0, sizeof( SOCKADDR_IN ) );
    serverAddr.sin_family = AF_INET;
    inet_pton( AF_INET, "127.0.0.1", &serverAddr.sin_addr );
    serverAddr.sin_port = htons( SERVER_PORT );
    
    int ret = WSAConnect( serverSocket, reinterpret_cast<sockaddr*>( &serverAddr ), sizeof( serverAddr ), 0, 0, 0, 0 );

    u_long flagOn = 1;
    ioctlsocket( serverSocket, FIONBIO, &flagOn );
}

UChattingInstance::~UChattingInstance()
{
    closesocket( serverSocket );
}

void UChattingInstance::Init()
{
    
}
