// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Networking.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "ChattingInstance.generated.h"


UCLASS()
class CHATTING_API UChattingInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UChattingInstance();
	~UChattingInstance();

	FSocket* m_serverSocket;
};
