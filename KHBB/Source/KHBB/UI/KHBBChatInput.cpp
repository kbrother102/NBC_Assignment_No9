// Fill out your copyright notice in the Description page of Project Settings.


#include "KHBBChatInput.h"

#include "Components/EditableTextBox.h"
#include "KHBB/Player/KHBBPlayerController.h"

void UKHBBChatInput::NativeConstruct()
{
	Super::NativeConstruct();
	if (!ChatInputBox->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted))
	{
		ChatInputBox->OnTextCommitted.AddDynamic(this, &ThisClass::UKHBBChatInput::OnChatInputTextCommitted);
	}
	if (!AnswerInputBox->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnAnswerInputTextCommitted))
	{
		AnswerInputBox->OnTextCommitted.AddDynamic(this, &ThisClass::UKHBBChatInput::OnAnswerInputTextCommitted);
	}
}

void UKHBBChatInput::NativeDestruct()
{
	Super::NativeDestruct();
	if (ChatInputBox->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted))
	{
		ChatInputBox->OnTextCommitted.RemoveDynamic(this, &ThisClass::UKHBBChatInput::OnChatInputTextCommitted);
	}
	if (!AnswerInputBox->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnAnswerInputTextCommitted))
	{
		AnswerInputBox->OnTextCommitted.AddDynamic(this, &ThisClass::UKHBBChatInput::OnAnswerInputTextCommitted);
	}
}

void UKHBBChatInput::OnChatInputTextCommitted(const FText& InputText, ETextCommit::Type CommitMethod)
{
	//문자열을 받아서 플레이어컨트롤러에게 주기
	if (CommitMethod == ETextCommit::OnEnter)
	{
		if (GetOwningPlayer())
		{
			AKHBBPlayerController* KHBPlayerController = Cast<AKHBBPlayerController>(GetOwningPlayer());
			if (KHBPlayerController)
			{
				KHBPlayerController->SetChattingMessage(InputText.ToString());
				//빈칸만들기
				ChatInputBox->SetText(FText());
			}
		}

	}
}

void UKHBBChatInput::OnAnswerInputTextCommitted(const FText& InputText, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		if (GetOwningPlayer())
		{
			AKHBBPlayerController* KHBPlayerController = Cast<AKHBBPlayerController>(GetOwningPlayer());
			if (KHBPlayerController)
			{
				KHBPlayerController->SetAnswerMessage(InputText.ToString());
				//빈칸만들기
				AnswerInputBox->SetText(FText());
			}
		}

	}
}
