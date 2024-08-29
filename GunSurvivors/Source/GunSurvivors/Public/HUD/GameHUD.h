// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EscOverlay.h"
#include "GameFramework/HUD.h"
#include "GameHUD.generated.h"

class UGameOverlay;

UCLASS()
class GUNSURVIVORS_API AGameHUD : public AHUD
{
	GENERATED_BODY()

public:
	void ShowEscMenu();
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "GunSurvivors")
	TSubclassOf<UGameOverlay> GunSurvivorsOverlayClass;
	UPROPERTY(EditDefaultsOnly, Category = "GunSurvivors")
	TSubclassOf<UEscOverlay> EscOverlayClass;
	
	UPROPERTY(VisibleDefaultsOnly, Category = "GunSurvivors")
	UGameOverlay* GunSurvivorsOverlay;
	UPROPERTY(VisibleDefaultsOnly, Category = "GunSurvivors")
	UEscOverlay* EscOverlay;

	bool bIsEscMenuVisible = false;

public:
	FORCEINLINE UGameOverlay* GetGunSurvivorsOverlay() const { return GunSurvivorsOverlay; }
};
