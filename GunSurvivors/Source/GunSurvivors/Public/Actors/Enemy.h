// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "Pawns/TopDownCharacter.h"
#include "Enemy.generated.h"

//动态多播委托
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEnemyDieDelegate);

UCLASS()
class GUNSURVIVORS_API AEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemy();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void FollowPlayer(float DeltaTime);
	void Rotate(const FVector& MovementDirection);
	void Die();
	void OnDestroyEnemyTimerTimeout();
	void PlaySound(USoundBase* SoundToPlay);

	/* <Components> */
	//教程代码不太好，这里不想再修改了
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enemy|Components")
	UCapsuleComponent* CapsuleComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enemy|Components")
	UPaperFlipbookComponent* EnemyFlipBookComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Movement")
	UPaperFlipbook* EnemyDeadFlipBook;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enemy|Movement")
	ATopDownCharacter* TargetPlayer;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enemy|Movement")
	bool bCanFollowPlayer = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enemy|Movement")
	bool bIsAlive = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Movement")
	float FollowSpeed = 30.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Movement")
	float DestroyEnemyTime = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Movement")
	float FollowStopDistance = 20.0f;//距离小于该值则停止移动
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Movement")
	USoundBase* EnemyDieSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Attributes")
	int32 AttackDamage = 10;

	FTimerHandle DestroyEnemyTimerHandle;
	FEnemyDieDelegate EnemyDieDelegate;
	/* </Components> */
};
