// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DesertRacerGameMode.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "PaperSpriteComponent.h"
#include "Obstacle.generated.h"

UCLASS()
class DESERTRACER_API AObstacle : public AActor
{
	GENERATED_BODY()
	
public:
	/* Components */
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = "Obstacle Components")
	UCapsuleComponent* CapsuleComponent;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = "Obstacle Components")
	UPaperSpriteComponent* ObstacleSpriteComponent;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Obstacle Components")
	USoundBase* HitSound;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Obstacle Components")
	bool bIsFinishLine = false;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Obstacle Components")
	bool bIsStartLine = false;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Obstacle Components")
	bool bIsTire = false;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "GameMode")
	ADesertRacerGameMode* MyGameMode;
	
	AObstacle();

	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//UFUNCTION()
	//void EndOverlap();

};
