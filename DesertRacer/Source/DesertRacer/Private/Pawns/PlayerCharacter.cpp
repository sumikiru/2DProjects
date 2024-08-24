// Fill out your copyright notice in the Description page of Project Settings.

#include "DesertRacer/Public/Pawns/PlayerCharacter.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;//自动控制玩家改为玩家0
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->SetCollisionProfileName(TEXT("Pawn"));
	RootComponent = CapsuleComponent;
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bDoCollisionTest = false;//对2D游戏不起作用所以关闭
	SpringArmComponent->SetUsingAbsoluteRotation(true);//将"变换"中的旋转改为绝对旋转（旋转类型由相对->世界场景）
	SpringArmComponent->SetWorldRotation(FRotator(0.f, 0.f, -89.999999f));//注意加上，要不然PlayerStart视角错误
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	CameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;//投射模式改为正交，适用于2d
	CameraComponent->OrthoWidth = 10000.f;//现在SpringArmComponent->TargetArmLength已经不起作用了
	CarSpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("CarSpriteComponent"));
	CarSpriteComponent->SetCollisionProfileName(TEXT("NoCollision"));//将CarSpriteComponent的碰撞预设设置为NoCollision
	CarSpriteComponent->SetTranslucentSortPriority(10);
	CarSpriteComponent->SetupAttachment(GetRootComponent());
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	this->Tags.Add(PlayerTagName);

	//添加映射上下文
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if(PlayerController && PlayerController->GetLocalPlayer())
	{
		UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if(InputSubsystem && InputMappingContext)
		{
			InputSubsystem->AddMappingContext(InputMappingContext, 0);
		}
	}

	//assign or bind function to MoveAction
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//添加按键映射
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if(EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		
	}
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MoveActionValue = Value.Get<FVector2D>();
	//GEngine->AddOnScreenDebugMessage(1, 10.f, FColor::Red, FString::Printf(TEXT("Move: X:%f, Y:%f"), MoveActionValue.X, MoveActionValue.Y));
	if(bCanMove)
	{
		//按下W/S
		if(FMath::Abs(MoveActionValue.Y) > 0.f)
		{
			const float DeltaTime = GetWorld()->DeltaTimeSeconds;

			//按下A/D
			if(FMath::Abs(MoveActionValue.X) > 0.f)
			{
				const float RotationAmount = -RotationSpeed * MoveActionValue.X * DeltaTime;//负
				const FRotator Rotator(RotationAmount, 0.f,0.f);
				AddActorWorldRotation(Rotator);
			}
			
			const FVector CurrentLocation = GetActorLocation();
			float FinalMovementSpeed = MovementSpeed;
			if(MoveActionValue.Y < 0.f)
			{
				FinalMovementSpeed *= 0.5f;
			}
			const FVector DistanceToMove = GetActorUpVector() * FinalMovementSpeed * MoveActionValue.Y * DeltaTime;//乘MoveActionValue.Y用来判断方向
			const FVector NewLocation = CurrentLocation + DistanceToMove;
			SetActorLocation(NewLocation);
		}
	}
}

