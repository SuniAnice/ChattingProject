// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChattingInstance.h"
#include <string>
#include <queue>
#include "NetworkManager.generated.h"


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
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	int DoRecv();

	void ProcessPacket();

	UFUNCTION( BlueprintImplementableEvent )
		void PrintBuffers( const TArray<FString>& str );

	UFUNCTION( BlueprintImplementableEvent )
		void PrintMyChat( const FString& str );

	UFUNCTION( BlueprintImplementableEvent )
		void QuitClient();

	UFUNCTION( BlueprintImplementableEvent )
	void PrintRoomList( const TArray<FString>& str );

	UFUNCTION( BlueprintImplementableEvent )
		void PrintUserList( const TArray<FString>& str );

	UFUNCTION( BlueprintImplementableEvent )
		void EnterRoom( const FString& str );

	UFUNCTION( BlueprintImplementableEvent )
		void ExitRoom();

	UFUNCTION( BlueprintImplementableEvent )
		void LoginSuccess( const FString& name );
};
