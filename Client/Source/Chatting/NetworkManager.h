// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChattingInstance.h"
#include <string>
#include <queue>
#include "NetworkManager.generated.h"


constexpr int BUFFER_SIZE = 1024;


UCLASS()
class CHATTING_API ANetworkManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANetworkManager();

	UPROPERTY( BlueprintReadWrite )
	UChattingInstance* m_instance;

protected:

	uint8 m_buffer[ BUFFER_SIZE ];

	bool m_isServerOff = false;

	int m_recvBytes = 0;

	std::queue<std::string> m_packets;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	int Send( const void* buffer, int32 size );

	int Recv();

	UFUNCTION( BlueprintImplementableEvent )
	void PrintBuffer( const FString& str );

	UFUNCTION( BlueprintImplementableEvent )
		void PrintBuffers( const TArray<FString>& str );

	UFUNCTION( BlueprintImplementableEvent )
		void QuitClient();

	void InitializeBuffer();

	void ProcessPacket();

	UFUNCTION( BlueprintImplementableEvent )
	void PrintRoomList( const TArray<FString>& str );

	UFUNCTION( BlueprintImplementableEvent )
		void PrintUserList( const TArray<FString>& str );

	UFUNCTION( BlueprintImplementableEvent )
		void EnterRoom( const FString& str );

	UFUNCTION( BlueprintImplementableEvent )
		void ExitRoom();
};
