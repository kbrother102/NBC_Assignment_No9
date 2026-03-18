// Fill out your copyright notice in the Description page of Project Settings.


#include "KHBBPlayerController.h"

#include "EngineUtils.h"
#include "KHBBPlayerState.h"
#include "KHBB/KHBB.h"
#include "KHBB/Game/KHBBGameModeBase.h"
#include "KHBB/UI/KHBBChatInput.h"
#include "KHBB/UI/KHBBNotification.h"


AKHBBPlayerController::AKHBBPlayerController()
{

}

void AKHBBPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsLocalPlayerController() == false)
	{
		return;
	}
	//UI만 조작할 수 있는 모드
	FInputModeUIOnly InputModeUIOnly;
	SetInputMode(InputModeUIOnly);
	
	//위젯 뷰포트에 띄우기
	if (ChatInputClass)
	{
		UKHBBChatInput* KHBBChatInputInstance = CreateWidget<UKHBBChatInput>(this, ChatInputClass);
		if (KHBBChatInputInstance)
		{
			KHBBChatInputInstance->AddToViewport();
		}
	}
	if (NotificationTextWidgetClass)
	{
		UKHBBNotification* KHBBNotificationInstance = CreateWidget<UKHBBNotification>(this, NotificationTextWidgetClass);
		if (KHBBNotificationInstance)
		{
			KHBBNotificationInstance->AddToViewport();
		}
	}
}

void AKHBBPlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
}

void AKHBBPlayerController::SetChattingMessage(const FString& InputChattingMessage)
{
	AKHBBPlayerState* KHBBPlayerState = GetPlayerState<AKHBBPlayerState>();
	
	if (KHBBPlayerState)
	{
		FString CombineMessage = KHBBPlayerState->GetPlayerInfoString() + InputChattingMessage;
		ChattingMessage = CombineMessage;
	}

	ServerRPCPrintMessageString(ChattingMessage);
}

void AKHBBPlayerController::SetAnswerMessage(const FString& InputAnswerMessage)
{
	AnswerMessage = InputAnswerMessage;
	
	ServerRPCAnswerString(AnswerMessage);
}

UUserWidget* AKHBBPlayerController::GetNotificationTextWidgetInstance()
{
	return NotificationTextWidgetInstance;
}

void AKHBBPlayerController::PrintChatMessage(const FString& MessageString)
{
	KHBBFunctionLibrary::MyPrintString(this, MessageString, 10.f);
}

void AKHBBPlayerController::ServerRPCAnswerString_Implementation(const FString& AnswerString)
{
	AKHBBGameModeBase* KHBBGameModebase = Cast<AKHBBGameModeBase>(GetWorld()->GetAuthGameMode());
	if (KHBBGameModebase)
	{
		KHBBGameModebase->PrintAnswerToClient(this, AnswerString); 
	}
}

void AKHBBPlayerController::ServerRPCPrintMessageString_Implementation(const FString& MessageString)
{
	AKHBBGameModeBase* KHBBGameModebase = Cast<AKHBBGameModeBase>(GetWorld()->GetAuthGameMode());
	if (KHBBGameModebase)
	{
		KHBBGameModebase->PrintMessageToClient(this, MessageString);
	}
}

void AKHBBPlayerController::ClientRPCPrintMessageString_Implementation(const FString& MessageString)
{
	PrintChatMessage(MessageString);
}
