// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverlay.generated.h"

class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOverlayChangedDelegate, int32, NewScore, int32, NewLevel, int32, NewHealth);

UCLASS()
class GUNSURVIVORS_API UGameOverlay : public UUserWidget
{
	GENERATED_BODY()

public:
	//UGameOverlay();是错误的，因为父类UUserWidget并没有默认构造函数UUserWidget()，所有不存在UGameOverlay()
	//这里NativeConstruct起到默认构造函数的作用
	virtual void NativeConstruct() override;
	
	//virtual void NativeConstruct() override;
	void SetScoreText(int32 NewScore) const;
	void SetLevelText(int32 NewLevel) const;
	void SetHealthText(int32 NewHealth) const;
	void InitOverlay();

	UFUNCTION()
	void OnUpdateOverlay(int32 NewScore, int32 NewLevel, int32 NewHealth);

	FOverlayChangedDelegate OverlayChangedDelegate;
	
private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ScoreText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* LevelText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* HealthText;
};
