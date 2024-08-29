// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EngineUtils.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "EscOverlay.generated.h"

UCLASS()
class GUNSURVIVORS_API UEscOverlay : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:
	UFUNCTION()
	void ResumeGame();
	UFUNCTION()
	void OpenSettings();
	UFUNCTION()
	void ExitGame();

	template<typename ActorClass>
	void TGetAllActorsOfClass(TArray<ActorClass*>& OutActors);
	
private:
	UPROPERTY(meta = (BindWidget))
	UButton* ResumeButton;
	UPROPERTY(meta = (BindWidget))
	UButton* SettingsButton;
	UPROPERTY(meta = (BindWidget))
	UButton* ExitButton;
};

//UGamePlayStatics::GetAllActorsOfClass(GetWorld(), AEnemySpawner::StaticClass(), EnemySpawners);是错误的，只能获得AActor类型的
//缺点是无法在蓝图中调用
template<typename ActorClass>
void UEscOverlay::TGetAllActorsOfClass(TArray<ActorClass*>& OutActors)
{
	OutActors.Reset();

	UWorld* World = GetWorld();
	if (World)
	{
		//如果获取类型的父类不是AActor而是UObject，则不使用TActorIterator，而是用TObjectIterator
		//需要包含头文件#include "EngineUtils.h"以实现UWorld*到int的转换
		for (TActorIterator<ActorClass> It(World); It; ++It)
		{
			OutActors.Add(*It);
		}
	}
}
