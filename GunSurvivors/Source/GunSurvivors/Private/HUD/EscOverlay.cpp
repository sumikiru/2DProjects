// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/EscOverlay.h"

#include "Actors/Enemy.h"
#include "Actors/EnemySpawner.h"
#include "HUD/GameHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Pawns/TopDownCharacter.h"

void UEscOverlay::NativeConstruct()
{
	Super::NativeConstruct();

	ResumeButton->OnClicked.AddDynamic(this, &UEscOverlay::ResumeGame);
	SettingsButton->OnClicked.AddDynamic(this, &UEscOverlay::OpenSettings);
	ExitButton->OnClicked.AddDynamic(this, &UEscOverlay::ExitGame);
}

void UEscOverlay::ResumeGame()
{
	AGameHUD* GameHUD = Cast<AGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if(GameHUD)
	{
		GameHUD->ShowEscMenu();//只是切换EscMenu开关
	}
}

void UEscOverlay::OpenSettings()
{
	//暂定为easier，简化游戏难度
	ATopDownCharacter* Player = Cast<ATopDownCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if(Player)
	{
		Player->ShootCooldownDurationInSeconds = 0.05f;//已经到了Upgrade设置的最小值，所以升级时不会改变
	}
	//获取场上的所有Enemy以及EnemySpawner，使用常规的GetActorsOfClass不能起作用
	TArray<AEnemySpawner*> EnemySpawners;
	TGetAllActorsOfClass(EnemySpawners);

	TArray<AEnemy*> Enemies;
	TGetAllActorsOfClass(Enemies);
	
	for(AEnemySpawner* EnemySpawner : EnemySpawners)
	{
		EnemySpawner->AttackDamage = 2;
	}
	for(AEnemy* Enemy : Enemies)
	{
		Enemy->AttackDamage = 2;
	}

	ResumeGame();
}

void UEscOverlay::ExitGame()
{
	UWorld* World = GetWorld();
	if(!World)
	{
		return;
	}
	APlayerController* PlayerController = World->GetFirstPlayerController();
	
	bool bIsQuitGame = World &&
		PlayerController &&
		ExitButton &&
		ExitButton->OnClicked.IsBound();
	if(bIsQuitGame)
	{
		UKismetSystemLibrary::QuitGame(
			World,
			PlayerController,
			EQuitPreference::Quit,
			false
		);
	}
}
