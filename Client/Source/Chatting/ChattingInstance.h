// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include <WS2tcpip.h>
#include "ChattingInstance.generated.h"


#pragma comment( lib, "Ws2_32.lib" )


UCLASS()
class CHATTING_API UChattingInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UChattingInstance();
	~UChattingInstance();
	virtual void Init() override;

	WSADATA wsaData;
	SOCKET serverSocket;
	SOCKADDR_IN serverAddr;
	FD_SET readSet, writeSet;
};
