// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperSpriteComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"


UCLASS()
class GUNSURVIVORS_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	ABullet();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void Launch(FVector2D Direction);
	void OnDeleteTimerTimeout();
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void DisableBullet();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USphereComponent* SphereComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UPaperSpriteComponent* BulletSpriteComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components|Movement")
	FVector2D MovementDirection = FVector2D(1.f, 0.f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components|Movement")
	float MovementSpeed = 200.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components|Movement")
	bool bIsLaunched = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components|Movement")
	bool bIsDisabled = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components|Movement")
	float DeleteTime = 3.0f;
	
	FTimerHandle DeleteBulletTimer;
};
