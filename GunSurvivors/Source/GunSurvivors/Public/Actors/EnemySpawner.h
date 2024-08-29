// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "GunSurvivorsGameMode.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

UCLASS()
class GUNSURVIVORS_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemySpawner();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void SetupEnemy(AEnemy* Enemy);
	void SpawnEnemy();
	void StartSpawning();
	void EndSpawning();
	void OnSpawnTimerTimeout();
	UFUNCTION()
	void OnEnemyDied();//必须要有与delegate相同数目的参数
	UFUNCTION()
	void OnPlayerDied();

	UPROPERTY(EditDefaultsOnly, Category = "Spawner")
	TSubclassOf<AEnemy>EnemyToSpawnClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	float SpawnDuration = 2.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	float MinSpawnDuration = 0.3f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	float DecreaseSpawnDurationByEachLevel = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	float MaxSpawnDistance = 200.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	float MinSpawnDistance = 50.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Spawner")
	int32 TotalEnemyCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	int32 DifficultySpawnCountLevel = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	int32 AttackDamage = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	int32 ScoreToAdd = 10;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Spawner")
	ATopDownCharacter* Player = nullptr;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Spawner")
	AGunSurvivorsGameMode* MyGameMode = nullptr;
	
	FTimerHandle SpawnEnemyTimer;

};
