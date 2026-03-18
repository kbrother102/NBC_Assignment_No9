// Fill out your copyright notice in the Description page of Project Settings.


#include "KHBBNotification.h"

#include "Components/TextBlock.h"
#include "KHBB/Player/KHBBPlayerController.h"

void UKHBBNotification::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	NotificationText->SetText(Cast<AKHBBPlayerController>(GetOwningPlayer())->NotificationText);
}
