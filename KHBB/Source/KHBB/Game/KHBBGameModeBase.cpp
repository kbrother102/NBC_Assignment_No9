// Fill out your copyright notice in the Description page of Project Settings.


#include "KHBBGameModeBase.h"

#include "EngineUtils.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "KHBB/Player/KHBBPlayerController.h"
#include "KHBB/Player/KHBBPlayerState.h"
#include "KHBB/UI/KHBBNotification.h"

void AKHBBGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	AKHBBPlayerController* KHBBPlayerController = Cast<AKHBBPlayerController>(NewPlayer);
	if (KHBBPlayerController)
	{
		AllPlayerControllers.Add(KHBBPlayerController);
		AKHBBPlayerState* KHBBPlayerState = KHBBPlayerController->GetPlayerState<AKHBBPlayerState>();
		if (KHBBPlayerState)
		{
			KHBBPlayerState->PlayerName = TEXT("Player ") + FString::FromInt(AllPlayerControllers.Num());
		}
	}
}

FString AKHBBGameModeBase::GenerateRandomNumberString()
{
	TArray<int32> NumberContainer;
	FString SecretNumber;
	FMath::RandInit(FDateTime::Now().GetTicks()); //현재 시간을 시드로 사용
	for (int i = 1; i <= 9; i++)
	{
		NumberContainer.Add(i);
	}

	//숫자 컨테이너에서 세개 뽑아서 넣기
	for (int i = 0; i < 3; i++)
	{
		int32 index = FMath::RandRange(0, NumberContainer.Num() - 1);
		int32 RandomNum = NumberContainer[index];
		SecretNumber.Append(FString::FromInt(RandomNum));
		NumberContainer.RemoveAt(index);
	}
	return SecretNumber;
}

bool AKHBBGameModeBase::IsGuessNumberString(const FString& InNumberString)
{
	bool bResult = true;
	if (InNumberString.IsEmpty()) // 비어있다면 컷
	{
		return false;
	}

	if (InNumberString.Len() != 3) // 길이 3 아니면 컷
	{
		return false;
	}
	TSet<TCHAR> UniqueTest;
	for (TCHAR Num : InNumberString) //문자 있거나 0 있으면 컷
	{
		if (Num == '0' || !FChar::IsDigit(Num))
		{
			return false;
		}
		UniqueTest.Add(Num);
	}

	if (UniqueTest.Num() < 3) // 중복이 있을때
	{
		return false;
	}

	return bResult;
}

void AKHBBGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	SecretNumberString = GenerateRandomNumberString();
	UE_LOG(LogTemp, Error, TEXT("%s"), *SecretNumberString);
}


void AKHBBGameModeBase::PrintMessageToClient(AKHBBPlayerController* TargetPlayers, const FString& MessageString)
{
	for (TActorIterator<AKHBBPlayerController> It(GetWorld()); It; ++It)
	{
		It->ClientRPCPrintMessageString(MessageString);; //자동으로 내부적으로 (*It) 해줌!
	}
}

void AKHBBGameModeBase::PrintAnswerToClient(AKHBBPlayerController* TargetPlayers, const FString& MessageString)
{
	if (IsGuessNumberString(MessageString) == true) //양식에 맞다면
	{
		AKHBBPlayerState* KHBBPlayerState = TargetPlayers->GetPlayerState<AKHBBPlayerState>();
		if (KHBBPlayerState)
		{
			
			if (KHBBPlayerState->IsOutGuess() == true)
			{
				TargetPlayers->ClientRPCPrintMessageString(TEXT("기회를 다 사용했습니다!")); // 해당 플레이어에게만 전송
				JudgeGame(TargetPlayers, KHBBPlayerState->bIsWin);
				return;
			}
			KHBBPlayerState->IncreaseCurrentGuessCount();

			FString PlayerInfo = KHBBPlayerState->GetPlayerInfoString();
			FString JudgeResultString = JudgeResult(SecretNumberString, MessageString);
			if (JudgeResultString == TEXT(" ThreeStrike!!"))
			{
				KHBBPlayerState->bIsWin = true;
			}
			FString AnswerJudge = PlayerInfo + MessageString + JudgeResultString;


			for (TActorIterator<AKHBBPlayerController> It(GetWorld()); It; ++It)
			{
				It->ClientRPCPrintMessageString(AnswerJudge);; //자동으로 내부적으로 (*It) 해줌!
			}

			JudgeGame(TargetPlayers, KHBBPlayerState->bIsWin);
		}
	}
	else
	{
		TargetPlayers->ClientRPCPrintMessageString(TEXT("똑바로 입력하세요 ..."));
	}
}

FString AKHBBGameModeBase::JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString)
{
	int32 StrikeCount = 0;
	int32 BallCount = 0;
	for (int i = 0; i < InSecretNumberString.Len(); i++)
	{
		if (InSecretNumberString[i] == InGuessNumberString[i])
		{
			StrikeCount++;
		}
		else
		{
			FString GuessNumberChar = FString() + InSecretNumberString[i];
			if (InGuessNumberString.Contains(GuessNumberChar))
			{
				BallCount++;
			}
		}
	}

	if (StrikeCount == 0 && BallCount == 0)
	{
		return TEXT(" OUT!");
	}

	if (StrikeCount == 3)
	{
		return TEXT(" ThreeStrike!!");
	}


	return FString::Printf(TEXT(" -> %d Strike %d Ball"), StrikeCount, BallCount);
}

void AKHBBGameModeBase::JudgeGame(AKHBBPlayerController* InChattingPlayerController, bool biswin)
{
	if (biswin == true)
	{
		FString WinnerString = InChattingPlayerController->GetPlayerState<AKHBBPlayerState>()->PlayerName +
			TEXT(" 가 이겼습니다!");

		for (const auto& PlayerController : AllPlayerControllers)
		{
			PlayerController->NotificationText = FText::FromString(WinnerString);
		}

		ResetGame();
	}
	else
	{
		bool bIsDraw = true;
		for (const auto& PlayerController : AllPlayerControllers)
		{
			if (PlayerController->GetPlayerState<AKHBBPlayerState>()->IsOutGuess() == false)
			{
				bIsDraw = false;
				break;
			}
		}
		
		if (bIsDraw == true)
		{
			for (const auto& PlayerController : AllPlayerControllers)
			{
				PlayerController->NotificationText = FText::FromString(TEXT("비겼습니다..."));
			}

			ResetGame();
		}
	}
}

void AKHBBGameModeBase::ResetGame()
{
	SecretNumberString = GenerateRandomNumberString();
	UE_LOG(LogTemp, Error, TEXT("%s"), *SecretNumberString);
	for (AKHBBPlayerController* KHBBPlayerController : AllPlayerControllers)
	{
		if (KHBBPlayerController)
		{
			//KHBBPlayerController->NotificationText = FText::FromString(TEXT("게임 시작!"));
			AKHBBPlayerState* KHBBPlayerState = KHBBPlayerController->GetPlayerState<AKHBBPlayerState>();
			if (KHBBPlayerState)
			{
				KHBBPlayerState->CurrentGuessCount = 0;
				KHBBPlayerState->bIsWin = false;
				
			}
		}
	}
}
