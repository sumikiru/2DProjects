// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/GameHUD.h"

#include "Blueprint/UserWidget.h"
#include "HUD/GameOverlay.h"
#include "Kismet/GameplayStatics.h"
#include "Pawns/TopDownCharacter.h"


void AGameHUD::ShowEscMenu()
{
	UWorld* World = GetWorld();
	ATopDownCharacter* Player = Cast<ATopDownCharacter>(World->GetFirstPlayerController()->GetPawn());
	if(!World || !Player)
	{
		return;
	}
	
	if(bIsEscMenuVisible && EscOverlay)
	{
		EscOverlay->RemoveFromParent();//关闭退出界面菜单
		bIsEscMenuVisible = false;
		GunSurvivorsOverlay->AddToViewport();//直接加回来即可
		UGameplayStatics::SetGlobalTimeDilation(World, 1.0f);

		Player->SwitchInputModeAndInputMappingContext(GunSurvivorsOverlay->TakeWidget());
	}
	else if(!bIsEscMenuVisible)
	{
		//开启退出界面菜单
		APlayerController* Controller = World->GetFirstPlayerController();
		if(Controller && EscOverlayClass)
		{
			EscOverlay = CreateWidget<UEscOverlay>(Controller, EscOverlayClass);//创建控件
			//init overlay
			GunSurvivorsOverlay->RemoveFromParent();
			EscOverlay->AddToViewport();
			bIsEscMenuVisible = true;
			UGameplayStatics::SetGlobalTimeDilation(World, 0.0001f);//只能在0.0001f到20.0000f之间

			Player->SwitchInputModeAndInputMappingContext(EscOverlay->TakeWidget());
		}
	}
}

void AGameHUD::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if(World)
	{
		APlayerController* Controller = World->GetFirstPlayerController();
	 	if(Controller && GunSurvivorsOverlayClass)
	 	{
	 		GunSurvivorsOverlay = CreateWidget<UGameOverlay>(Controller, GunSurvivorsOverlayClass);//创建控件
	 		//init overlay
	 		GunSurvivorsOverlay->InitOverlay();//包含AddToViewport
	 	}
	}
}
