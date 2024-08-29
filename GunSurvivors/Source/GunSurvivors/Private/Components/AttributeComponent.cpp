// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AttributeComponent.h"

#include "HUD/GameHUD.h"
#include "HUD/GameOverlay.h"

UAttributeComponent::UAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAttributeComponent::AddScore(int32 ScoreToAdd)
{
	Score = FMath::Clamp(Score + ScoreToAdd, 0, 99999);

	UpdateAttribute();
}

void UAttributeComponent::LoseHealth(int32 HealthToLose)
{
	Health = FMath::Clamp(Health - HealthToLose, 0, MaxHealth);

	UpdateAttribute();
}

void UAttributeComponent::Upgrade()
{
	Level++;
	MaxHealth = FMath::Clamp(MaxHealth + 50, 0, 9000);
	Health = MaxHealth;

	UpdateAttribute();
}

void UAttributeComponent::UpdateAttribute()
{
	//直接顺带把Overlay的一块实现了
	APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());//不能使用GetOwner(),返回值类型有误，无法成功cast
	if (PlayerController)
	{
		AGameHUD* GameHUD = Cast<AGameHUD>(PlayerController->GetHUD());
		UGameOverlay* GunSurvivorsOverlay = GameHUD->GetGunSurvivorsOverlay();
		if (GameHUD && GunSurvivorsOverlay)
		{
			GunSurvivorsOverlay->OverlayChangedDelegate.Broadcast(Score, Level, Health);
		}
	}
}


void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

