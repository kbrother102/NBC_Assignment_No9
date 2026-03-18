// Fill out your copyright notice in the Description page of Project Settings.


#include "KHBBPlayerState.h"

#include "Net/UnrealNetwork.h"

AKHBBPlayerState::AKHBBPlayerState() :
	CurrentGuessCount(0),
	MaxGuessCount(3),
	bIsWin(false)
{
	bReplicates = true;
}

void AKHBBPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, PlayerName);
	DOREPLIFETIME(ThisClass, CurrentGuessCount);
	DOREPLIFETIME(ThisClass, MaxGuessCount);
	DOREPLIFETIME(ThisClass, bIsWin);
}

bool AKHBBPlayerState::IsOutGuess()
{
	return CurrentGuessCount >= MaxGuessCount;
}

FString AKHBBPlayerState::GetPlayerInfoString() const
{
	//추후 횟수 추가~
	FString PlayerInfoString = PlayerName + FString::Printf(TEXT(" (%d / %d) : "), CurrentGuessCount, MaxGuessCount);
	return PlayerInfoString;
}

void AKHBBPlayerState::IncreaseCurrentGuessCount()
{
	CurrentGuessCount++;
}
