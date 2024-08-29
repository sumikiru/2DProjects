// Fill out your copyright notice in the Description page of Project Settings.


#include "GunSurvivorsGameMode.h"
#include "GunSurvivorsGameInstance.h"
#include "Kismet/GameplayStatics.h"

void AGunSurvivorsGameMode::RestartGame()
{
	GetWorldTimerManager().SetTimer(RestartTimerHandle, this, &AGunSurvivorsGameMode::OnRestartTimerTimeout, 1.f, false, TimeBeforeRestart);
}

void AGunSurvivorsGameMode::OnRestartTimerTimeout()
{
	UGunSurvivorsGameInstance* GameInstance = Cast<UGunSurvivorsGameInstance>(GetGameInstance());
	if(GameInstance)
	{
		GameInstance->ResetPlayerLocation();//仅用于获取RespawnLocation
	}
	UGameplayStatics::OpenLevel(GetWorld(), FName("MainLevel"));
	//设置Player—>SetActorLocation()在其对应的BeginPlay()中设置，以实现OpenLevel之后才改变位置
}

void AGunSurvivorsGameMode::BeginPlay()
{
	Super::BeginPlay();

	UGunSurvivorsGameInstance* GameInstance = Cast<UGunSurvivorsGameInstance>(GetGameInstance());
	if(GameInstance)
	{
		GameInstance->PlayBgm();
	}
}
