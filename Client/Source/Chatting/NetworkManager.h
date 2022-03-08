// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChattingInstance.h"
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

	uint8 m_buffer[ BUFFER_SIZE ];

	bool m_isServerOff = false;

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
		void QuitClient();

	void InitializeBuffer();
};
