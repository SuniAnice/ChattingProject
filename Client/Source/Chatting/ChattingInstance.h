// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Networking.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include <string>
#include <queue>
#include "ChattingInstance.generated.h"


constexpr int BUFFER_SIZE = 1024;


UCLASS()
class CHATTING_API UChattingInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UChattingInstance();
	~UChattingInstance();

	virtual void Init() override;

	virtual void Shutdown() override;

	bool m_isServerOff = false;

	FSocket* m_serverSocket = nullptr;

	int m_recvBytes = 0;

	uint8 m_buffer[ BUFFER_SIZE ];

	std::queue<std::string> m_packets;

	std::queue<std::string> m_sends;

	std::string m_nickname;

	// recv시 처리하고 남은 패킷 덩어리
	std::string m_leftovers;

	int Send( std::string& buffer, int32 size );

	int Recv();

	void InitializeBuffer();

};
