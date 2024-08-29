// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GunSurvivorsGameMode.generated.h"

/**
 * 
 */
UCLASS()
class GUNSURVIVORS_API AGunSurvivorsGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	void RestartGame();
	void OnRestartTimerTimeout();

	UPROPERTY(EditAnywhere, BlueprintReadWrite ,Category = "GunSurvivorsGameBasicSettings")
	float TimeBeforeRestart = 2.5f;

	FTimerHandle RestartTimerHandle;
	
protected:
	virtual void BeginPlay() override;
};
