// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NetworkManager.h"
#include "ChattingWidget.generated.h"


UCLASS()
class CHATTING_API UChattingWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION( BlueprintCallable )
		void SendChat( FString string );

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
		ANetworkManager* m_manager;
};
