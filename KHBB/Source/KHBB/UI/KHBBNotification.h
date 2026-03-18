// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KHBBNotification.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class KHBB_API UKHBBNotification : public UUserWidget
{
	GENERATED_BODY()
	
	public:
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> NotificationText;
};
