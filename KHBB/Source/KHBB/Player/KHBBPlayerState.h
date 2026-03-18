// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "KHBBPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class KHBB_API AKHBBPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	AKHBBPlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	bool IsOutGuess();
	FString GetPlayerInfoString() const;
	void IncreaseCurrentGuessCount();
	
	UPROPERTY(Replicated)
	bool bIsWin;
	UPROPERTY(Replicated)
	FString PlayerName;
	UPROPERTY(Replicated)
	int32 CurrentGuessCount;
	UPROPERTY(Replicated)
	int32 MaxGuessCount;
};
