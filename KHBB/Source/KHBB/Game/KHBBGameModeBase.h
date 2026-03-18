// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "KHBBGameModeBase.generated.h"

class AKHBBPlayerController;
/**
 * 
 */
UCLASS()
class KHBB_API AKHBBGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	virtual void OnPostLogin(AController* NewPlayer) override;
	FString GenerateRandomNumberString();
	
	bool IsGuessNumberString(const FString& InNumberString); // 정답 양식에 맞는 지 확인
	
	virtual void BeginPlay() override;
	void PrintMessageToClient(AKHBBPlayerController* TargetPlayers, const FString& MessageString);// 클라이언트에게 전파하기 (기존의 Playercontroller로직을 여기로)
	void PrintAnswerToClient(AKHBBPlayerController* TargetPlayers, const FString& MessageString);
	FString JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString);
	void JudgeGame(AKHBBPlayerController* InChattingPlayerController, bool biswin);
	void ResetGame();

	
protected:
	TArray<TObjectPtr<AKHBBPlayerController>> AllPlayerControllers;
	
	FString SecretNumberString;
};
