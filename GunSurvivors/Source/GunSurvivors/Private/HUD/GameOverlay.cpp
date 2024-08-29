// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/GameOverlay.h"

#include "Components/TextBlock.h"

void UGameOverlay::NativeConstruct()
{
	Super::NativeConstruct();
	
	this->OverlayChangedDelegate.AddDynamic(this, &UGameOverlay::OnUpdateOverlay);
}

void UGameOverlay::SetScoreText(int32 NewScore) const
{
	if(NewScore >= 0)
	{
		FString NewText = "Score: " + FString::FromInt(NewScore);
		//FString NewText = FString::Printf(TEXT("Score: %d"), Score);
		ScoreText->SetText(FText::FromString(NewText));
	}
}

void UGameOverlay::SetLevelText(int32 NewLevel) const
{
	if(NewLevel >= 1)
	{
		FString NewText = "Lv : " + FString::FromInt(NewLevel);
		LevelText->SetText(FText::FromString(NewText));
	}
}

void UGameOverlay::SetHealthText(int32 NewHealth) const
{
	if(NewHealth >= 0)
	{
		FString NewText = "HP : " + FString::FromInt(NewHealth);
		HealthText->SetText(FText::FromString(NewText));
	}
}

void UGameOverlay::InitOverlay()
{
	AddToViewport();
	
	SetScoreText(0);
	SetLevelText(1);
	SetHealthText(100);
}

void UGameOverlay::OnUpdateOverlay(int32 NewScore, int32 NewLevel, int32 NewHealth)
{
	SetScoreText(NewScore);
	SetLevelText(NewLevel);
	SetHealthText(NewHealth);
}
