// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GunSurvivorsGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class GUNSURVIVORS_API UGunSurvivorsGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	void PlayBgm();
	void ResetPlayerLocation();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite ,Category = "GunSurvivorsGameBasicSettings")
	USoundBase* BgmSound;
	UPROPERTY()
	UAudioComponent* BgmAudio;
	UPROPERTY(EditAnywhere, BlueprintReadWrite ,Category = "GunSurvivorsGameBasicSettings")
	FVector PlayerRespawnLocation = FVector::ZeroVector;

public:
	FORCEINLINE void SetPlayerRespawnLocation(const FVector& RespawnLocation) { PlayerRespawnLocation = RespawnLocation; }
	FORCEINLINE FVector GetPlayerRespawnLocation() const { return PlayerRespawnLocation; }
};
