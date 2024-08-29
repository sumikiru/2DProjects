// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "PaperFlipbookComponent.h"
#include "PaperSpriteComponent.h"
#include "Components/AttributeComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"
#include "TopDownCharacter.generated.h"

class ABullet;
class UCapsuleComponent;
class UInputComponent;
class UInputMappingContext;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FScoreChangedDelegate, int32, ScoreToAdd);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerDieDelegate);

UCLASS()
class GUNSURVIVORS_API ATopDownCharacter : public APawn
{
	GENERATED_BODY()

public:
	ATopDownCharacter();
	
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	/* <Input> */
	void MoveTriggered(const FInputActionValue& Value);
	void MoveCompleted(const FInputActionValue& Value);
	void OnShootCoolDownTimerTimeout();
	void SpawnBullet();
	void Shoot(const FInputActionValue& Value);
	void PlaySound(USoundBase* SoundToPlay);
	/* </Input> */

	/* <Movement> */
	bool IsInMapHorizontalBounds(float XPos);
	bool IsInMapVerticalBounds(float ZPos);
	void RotateGun();
	/* </Movement> */

	/* <UI> */
	void PressEsc();
	void SwitchInputModeAndInputMappingContext(TSharedPtr<SWidget> InWidgetToFocus);
	/* </UI> */

	UFUNCTION()
	void OnScoreChanged(int32 ScoreToAdd);
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/* <Components> */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerComponents")
	UCapsuleComponent* CapsuleComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerComponents")
	UPaperFlipbookComponent* CharacterFlipBookComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerComponents")
	USceneComponent* GunParentSceneComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerComponents")
	UPaperSpriteComponent* GunSpriteComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerComponents")
	USceneComponent* BulletSpawnPosition;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerComponents")
	UPaperFlipbookComponent* GunFlipBookComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerComponents|Attributes")
	UAttributeComponent* Attribute;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerComponents")
	UPaperFlipbook* IdleFlipBook;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerComponents")
	UPaperFlipbook* RunFlipBook;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerComponents|Movement")
	FVector2D HorizontalLimits = FVector2D(-130.f, 130.f);
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerComponents|Movement")
	FVector2D VerticalLimits = FVector2D(-130.f, 130.f);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerComponents|Input")
	UInputMappingContext* InputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerComponents|Input")
	UInputMappingContext* EscMenuInputMappingContext;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerComponents|Input")
	bool bIsPausing = true;//BeginPlay()时会切换为false
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerComponents|Movement")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerComponents|Movement")
	UInputAction* ShootAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerComponents|Movement")
	UInputAction* EscAction;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerComponents|Movement")
	bool bIsAlive = true;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerComponents|Movement")
	bool bCanMove = true;
	UPROPERTY(EditDefaultsOnly, Category = "PlayerComponents|Movement|Shoot")
	TSubclassOf<ABullet> BulletToSpawnClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerComponents|Movement|Shoot")
	bool bCanShoot = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerComponents|Movement")
	float MoveSpeed = 100.0f;
	UPROPERTY(BlueprintReadOnly, Category = "PlayerComponents|Movement")
	FVector2D MovementDirection;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerComponents|Movement|Shoot")
	float ShootCooldownDurationInSeconds = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerComponents|Movement|Shoot")
	USoundBase* BulletShootSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerComponents|Movement|Shoot")
	USoundBase* PlayerDieSound;

	FTimerHandle ShootCooldownTimer;
	FScoreChangedDelegate ScoreChangedDelegate;
	FPlayerDieDelegate PlayerDieDelegate;
	/* </Components> */
};
