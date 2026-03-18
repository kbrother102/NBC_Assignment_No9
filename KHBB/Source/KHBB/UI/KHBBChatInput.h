// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KHBBChatInput.generated.h"

class UEditableTextBox;
/**
 * 
 */
UCLASS()
class KHBB_API UKHBBChatInput : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	
protected:
	UFUNCTION()
	void OnChatInputTextCommitted(const FText& InputText, ETextCommit::Type CommitMethod);
	UFUNCTION()
	void OnAnswerInputTextCommitted(const FText& InputText, ETextCommit::Type CommitMethod);
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> AnswerInputBox;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> ChatInputBox;
};
