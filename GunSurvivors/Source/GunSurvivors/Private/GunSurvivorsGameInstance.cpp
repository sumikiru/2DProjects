// Fill out your copyright notice in the Description page of Project Settings.


#include "GunSurvivorsGameInstance.h"

#include "Kismet/GameplayStatics.h"
#include "Pawns/TopDownCharacter.h"

void UGunSurvivorsGameInstance::PlayBgm()
{
	if(!BgmAudio && BgmSound)
	{
		//Note: 记得先将选择的BgmSound的"音效"->“正在循环Is Looping”设置为true
		//重新OpenLevel会导致GameMode.h中的所有数值重置，而GameInstance不会;
		//GameInstance: high-level manager object for an instance of the running game.
		//Spawned at game creation and not destroyed until game instance is shut down.
		//Running as a standalone game, there will be one of these.
		BgmAudio = UGameplayStatics::SpawnSound2D(GetWorld(), BgmSound, 1.f, 1.f, 0.f, nullptr, true, true);//会自动销毁
	}
}

void UGunSurvivorsGameInstance::ResetPlayerLocation()
{
	ATopDownCharacter* Player = Cast<ATopDownCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	//游戏刚开始时的位置由GameInstance记录的默认值决定
	if (Player)
	{
		PlayerRespawnLocation = Player->GetActorLocation();
	}
}
