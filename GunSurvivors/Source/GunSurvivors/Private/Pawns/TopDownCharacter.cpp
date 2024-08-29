// Fill out your copyright notice in the Description page of Project Settings.


#include "GunSurvivors/Public/Pawns/TopDownCharacter.h"
#include "EnhancedInputComponent.h"
#include "GunSurvivorsGameInstance.h"
#include "Actors/Bullet.h"
#include "Actors/Enemy.h"
#include "Components/CapsuleComponent.h"
#include "HUD/GameHUD.h"
#include "HUD/GameOverlay.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ATopDownCharacter::ATopDownCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	SetRootComponent(CapsuleComponent);
	CapsuleComponent->SetCollisionProfileName(TEXT("Pawn"));
	CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &ATopDownCharacter::OnBeginOverlap);

	CharacterFlipBookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("CharacterFlipBookComponent"));
	CharacterFlipBookComponent->SetupAttachment(RootComponent);
	CharacterFlipBookComponent->SetCollisionProfileName(TEXT("NoCollision"));
	CharacterFlipBookComponent->TranslucencySortPriority = 0;

	GunParentSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("GunParent"));
	GunParentSceneComponent->SetupAttachment(RootComponent);

	GunSpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("GunSpriteComponent"));
	GunSpriteComponent->SetupAttachment(GunParentSceneComponent);
	GunSpriteComponent->SetCollisionProfileName(TEXT("NoCollision"));
	GunSpriteComponent->TranslucencySortPriority = 5;

	BulletSpawnPosition = CreateDefaultSubobject<USceneComponent>(TEXT("BulletSpawnPosition"));
	BulletSpawnPosition->SetupAttachment(GunSpriteComponent);

	Attribute = CreateDefaultSubobject<UAttributeComponent>(TEXT("PlayerAttribute"));//创建即可，无需SetupAttachment

	this->AutoReceiveInput = EAutoReceiveInput::Player0;//不需要再在PlayStart中设置了
	this->Tags.AddUnique(TEXT("Player"));
	this->ScoreChangedDelegate.AddDynamic(this, &ATopDownCharacter::OnScoreChanged);
}

void ATopDownCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 注册映射上下文
	AGameHUD* GameHUD = Cast<AGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if(GameHUD)
	{
		SwitchInputModeAndInputMappingContext(GameHUD->GetGunSurvivorsOverlay()->TakeWidget());
	}

	UGunSurvivorsGameInstance* GameInstance = Cast<UGunSurvivorsGameInstance>(GetGameInstance());
	if(GameInstance)
	{
		this->SetActorLocation(GameInstance->GetPlayerRespawnLocation());
	}
}

void ATopDownCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RotateGun();
}

void ATopDownCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* PlayerEnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if(PlayerEnhancedInputComponent)
	{
		PlayerEnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATopDownCharacter::MoveTriggered);
		PlayerEnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ATopDownCharacter::MoveCompleted);
		PlayerEnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Canceled, this, &ATopDownCharacter::MoveCompleted);
		PlayerEnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &ATopDownCharacter::Shoot);
		PlayerEnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &ATopDownCharacter::Shoot);
		PlayerEnhancedInputComponent->BindAction(EscAction, ETriggerEvent::Started, this, &ATopDownCharacter::PressEsc);
	}
}

void ATopDownCharacter::RotateGun()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if(PlayerController)
	{
		FVector MouseWorldLocation, MouseWorldDirection;
		PlayerController->DeprojectMousePositionToWorld(MouseWorldLocation, MouseWorldDirection);

		const FVector CurrentLocation = GetActorLocation();
		const FVector Start = FVector(CurrentLocation.X, 0.f, CurrentLocation.Z);
		const FVector Target = FVector(MouseWorldLocation.X, 0.f, MouseWorldLocation.Z);
		const FRotator GunParentRotator = UKismetMathLibrary::FindLookAtRotation(Start, Target);
		GunParentSceneComponent->SetRelativeRotation(GunParentRotator);
	}
}

void ATopDownCharacter::PressEsc()
{
	AGameHUD* GameHUD = Cast<AGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if(GameHUD)
	{
		GameHUD->ShowEscMenu();
	}
}

void ATopDownCharacter::SwitchInputModeAndInputMappingContext(TSharedPtr<SWidget> InWidgetToFocus)
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if(PlayerController)
	{
		PlayerController->SetShowMouseCursor(true);
		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(InWidgetToFocus);
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways); //始终处于Viewport内
		InputMode.SetHideCursorDuringCapture(false); //点击时不会隐藏鼠标
		PlayerController->SetInputMode(InputMode);
		
		UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if(InputSubsystem)
		{
			if(bIsPausing)
			{
				InputSubsystem->RemoveMappingContext(EscMenuInputMappingContext);//This is safe to call even if the context is not applied
				InputSubsystem->AddMappingContext(InputMappingContext, 0);
				bIsPausing = false;
			}
			else
			{
				InputSubsystem->RemoveMappingContext(InputMappingContext);
				InputSubsystem->AddMappingContext(EscMenuInputMappingContext, 0);
				bIsPausing = true;
			}
		}
	}
}

void ATopDownCharacter::OnScoreChanged(int32 ScoreToAdd)
{
	//已经在AttributeComponents中将Overlay更新同步实现了
	if(Attribute)
	{
		Attribute->AddScore(ScoreToAdd);
		if(Attribute->GetScore() >= Attribute->GetUpgradeScoreCost() * Attribute->GetLevel())
		{
			//Upgrade
			Attribute->Upgrade();
			ShootCooldownDurationInSeconds = FMath::Clamp(ShootCooldownDurationInSeconds / 1.5f, 0.05f, 2.0f);
		}
	}
}

void ATopDownCharacter::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEnemy* Enemy = Cast<AEnemy>(OtherActor);
	if (Enemy && Enemy->bIsAlive)
	{
		if (bIsAlive && Attribute)
		{
			Attribute->LoseHealth(Enemy->AttackDamage);
			//玩家死亡
			if (Attribute->GetHealth() <= 0)
			{
				bIsAlive = false;
				bCanMove = false;
				bCanShoot = false;

				PlaySound(PlayerDieSound);
				PlayerDieDelegate.Broadcast();
			}
		}
	}
}

void ATopDownCharacter::MoveTriggered(const FInputActionValue& Value)
{
	FVector2D MoveActionValue = Value.Get<FVector2D>();
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("X: %f, Y: %f"), MoveActionValue.X, MoveActionValue.Y));
	if(bCanMove)
	{
		MovementDirection = MoveActionValue;
		CharacterFlipBookComponent->SetFlipbook(RunFlipBook);

		//rotate
		const FVector FlipBookScale = CharacterFlipBookComponent->GetComponentScale();//也可以更改Rotation
		bool bShouldRotate = MovementDirection.X < 0.f && FlipBookScale.X > 0.f ||
			MovementDirection.X > 0.f && FlipBookScale.X < 0.f;
		if(bShouldRotate)
		{
			const FVector NewScale = FVector(-FlipBookScale.X, FlipBookScale.Y, FlipBookScale.Z);
			CharacterFlipBookComponent->SetWorldScale3D(NewScale);
		}

		//move
		if(MovementDirection.Length() > 0.0f)
		{
			MovementDirection = MovementDirection.GetSafeNormal();
			// if(MovementDirection.Length() > 1.0f)
			// {
			// 	MovementDirection.Normalize();
			// }
			const float DeltaTime = GetWorld()->DeltaTimeSeconds;
			const FVector2D DistanceToMove = MovementDirection * MoveSpeed * DeltaTime;
			const FVector CurrentLocation = GetActorLocation();
			FVector NewLocation = CurrentLocation + FVector(DistanceToMove.X, 0.f, DistanceToMove.Y);///注意，拌合输入轴值是建立在XY上的，但是角色移动是基于XZ的
			if(!IsInMapHorizontalBounds(NewLocation.X))
			{
				NewLocation.X = FMath::Clamp(NewLocation.X, HorizontalLimits.X, HorizontalLimits.Y);
			}
			if(!IsInMapVerticalBounds(NewLocation.Z))
			{
				NewLocation.Z = FMath::Clamp(NewLocation.Z, VerticalLimits.X, VerticalLimits.Y);
			}

			SetActorLocation(NewLocation);
		}
	}
}

void ATopDownCharacter::MoveCompleted(const FInputActionValue& Value)
{
	MovementDirection = FVector2D(0.0f, 0.0f);
	if(bIsAlive)
	{
		CharacterFlipBookComponent->SetFlipbook(IdleFlipBook);
	}
}

void ATopDownCharacter::OnShootCoolDownTimerTimeout()
{
	if(bIsAlive)
	{
		bCanShoot = true;
	}
}

void ATopDownCharacter::SpawnBullet()
{
	//spawn bullet actor
	const FVector LocationToSpawn = BulletSpawnPosition->GetComponentLocation();
	const FRotator RotationToSpawn = FRotator(0.f, 0.f, 0.f);
	ABullet* Bullet = GetWorld()->SpawnActor<ABullet>(BulletToSpawnClass, LocationToSpawn, RotationToSpawn);
	//check(Bullet);//最终发行代码不应包括此行

	//get mouse location
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if(PlayerController)
	{
		FVector MouseWorldLocation, MouseWorldDirection;
		PlayerController->DeprojectMousePositionToWorld(MouseWorldLocation, MouseWorldDirection);

		//calculate bullet direction
		FVector CurrentLocation= GetActorLocation();
		FVector2D BulletDirection = FVector2D(MouseWorldLocation.X - CurrentLocation.X, MouseWorldLocation.Z - CurrentLocation.Z);
		BulletDirection = BulletDirection.GetSafeNormal();

		//launch the bullet
		Bullet->Launch(BulletDirection);
	}
}

void ATopDownCharacter::Shoot(const FInputActionValue& Value)
{
	if(bCanShoot)
	{
		bCanShoot = false;

		PlaySound(BulletShootSound);
		SpawnBullet();
		
		GetWorldTimerManager().SetTimer(ShootCooldownTimer, this, &ATopDownCharacter::OnShootCoolDownTimerTimeout, 1.f, false, ShootCooldownDurationInSeconds);
	}
}

void ATopDownCharacter::PlaySound(USoundBase* SoundToPlay)
{
	if(SoundToPlay && GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundToPlay);
	}
}

bool ATopDownCharacter::IsInMapHorizontalBounds(float XPos)
{
	return XPos > HorizontalLimits.X &&
		XPos < HorizontalLimits.Y;
}

bool ATopDownCharacter::IsInMapVerticalBounds(float ZPos)
{
	return ZPos > VerticalLimits.X &&
		ZPos < VerticalLimits.Y;
}

