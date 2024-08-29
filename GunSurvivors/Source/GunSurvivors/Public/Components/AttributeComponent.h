// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GUNSURVIVORS_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttributeComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AddScore(int32 ScoreToAdd);
	void LoseHealth(int32 HealthToLose);
	void Upgrade();
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "PlayerComponents|Attributes")
	int32 Score = 0;
	UPROPERTY(EditAnywhere, Category = "PlayerComponents|Attributes")
	int32 Level = 1;
	UPROPERTY(EditAnywhere, Category = "PlayerComponents|Attributes")
	int32 Health = 100;
	UPROPERTY(EditAnywhere, Category = "PlayerComponents|Attributes")
	int32 MaxHealth = 100;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = "PlayerComponents|Attributes")
	int32 UpgradeScoreCost = 100;

	void UpdateAttribute();

public:
	FORCEINLINE int32 GetScore() const { return Score; }
	FORCEINLINE int32 GetLevel() const { return Level; }
	FORCEINLINE int32 GetHealth() const { return Health; }
	FORCEINLINE int32 GetUpgradeScoreCost() const { return UpgradeScoreCost; }
	FORCEINLINE void SetScore(int32 NewScore) { Score = NewScore; }
	FORCEINLINE void SetLevel(int32 NewLevel) { Level = NewLevel; }
	FORCEINLINE void SetHealth(int32 NewHealth) { Health = NewHealth; }
};
