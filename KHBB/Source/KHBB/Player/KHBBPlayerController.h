// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "KHBBPlayerController.generated.h"

class UKHBBChatInput;
/**
 * 
 */
UCLASS()
class KHBB_API AKHBBPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AKHBBPlayerController();
	
	
	
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	void SetChattingMessage(const FString& InputChattingMessage);
	void SetAnswerMessage(const FString& InputAnswerMessage);
	FString ChattingMessage;
	FString AnswerMessage;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UKHBBChatInput> ChatInputClass;
	UPROPERTY()
	TObjectPtr<UKHBBChatInput> ChatInputInstance;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> NotificationTextWidgetClass;
	UPROPERTY()
	TObjectPtr<UUserWidget> NotificationTextWidgetInstance;
	
	UUserWidget* GetNotificationTextWidgetInstance();
	

	void PrintChatMessage(const FString& MessageString);
	
	UPROPERTY(Replicated, BlueprintReadOnly)
	FText NotificationText;
	
	UFUNCTION(Server, Reliable)
	void ServerRPCPrintMessageString(const FString& MessageString);
	UFUNCTION(Client, Reliable)
	void ClientRPCPrintMessageString(const FString& MessageString);
	
	UFUNCTION(Server, Reliable)
	void ServerRPCAnswerString(const FString& AnswerString);
};
