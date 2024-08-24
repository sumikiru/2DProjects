// Fill out your copyright notice in the Description page of Project Settings.


#include "DesertRacerGameMode.h"

#include "Kismet/GameplayStatics.h"

void ADesertRacerGameMode::ResetGameLevel(bool bIsWin)
{
	float ResetTime = bIsWin ? WinResetTime : LoseResetTime;

	GetWorldTimerManager().SetTimer(ResetGameTimer, this, &ADesertRacerGameMode::OnResetGameTimerTimeout, 1.f, false, ResetTime);
}

void ADesertRacerGameMode::OnResetGameTimerTimeout()
{
	UWorld* World = GetWorld();
	if(!World) return;
	UGameplayStatics::OpenLevel(World, FName("MainLevel"));
}
