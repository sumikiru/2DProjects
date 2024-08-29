// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Enemy.h"

#include "Kismet/GameplayStatics.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	SetRootComponent(CapsuleComponent);
	CapsuleComponent->SetCollisionProfileName("OverlapAllDynamic");
	
	EnemyFlipBookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("EnemyFlipBookComponent"));
	EnemyFlipBookComponent->SetCollisionProfileName("NoCollision");
	EnemyFlipBookComponent->SetTranslucentSortPriority(-4);
	EnemyFlipBookComponent->SetupAttachment(RootComponent);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FollowPlayer(DeltaTime);
}

void AEnemy::FollowPlayer(float DeltaTime)
{
	if(bIsAlive && bCanFollowPlayer && TargetPlayer)
	{
		const FVector CurrentLocation = GetActorLocation();
		const FVector TargetLocation = TargetPlayer->GetActorLocation();
		FVector DirectionToPlayer = TargetLocation - CurrentLocation;
		const float DistanceToPlayer = DirectionToPlayer.Size();
		if(DistanceToPlayer > FollowStopDistance)
		{
			DirectionToPlayer = DirectionToPlayer.GetSafeNormal();
			Rotate(DirectionToPlayer);
			
			const FVector NewLocation = CurrentLocation + DirectionToPlayer * FollowSpeed * DeltaTime;
			SetActorLocation(NewLocation);
		}
		else
		{
			//对玩家造成伤害
			bCanFollowPlayer = false;
			//TargetPlayer->ReceiveDamage(1.f);
			Destroy();
		}
	}
}

void AEnemy::Rotate(const FVector& MovementDirection)
{
	const FVector FlipBookScale = EnemyFlipBookComponent->GetComponentScale();//也可以通过更改Rotation
	bool bShouldRotate = MovementDirection.X < 0.f && FlipBookScale.X > 0.f ||
		MovementDirection.X > 0.f && FlipBookScale.X < 0.f;
	if(bShouldRotate)
	{
		const FVector NewScale = FVector(-FlipBookScale.X, FlipBookScale.Y, FlipBookScale.Z);
		EnemyFlipBookComponent->SetWorldScale3D(NewScale);
	}
}

void AEnemy::Die()
{
	if(!bIsAlive)
	{
		return;
	}
	bIsAlive = false;

	EnemyFlipBookComponent->SetFlipbook(EnemyDeadFlipBook);
	EnemyFlipBookComponent->SetTranslucentSortPriority(-5);

	PlaySound(EnemyDieSound);
	EnemyDieDelegate.Broadcast();//注意，涉及到委托
	
	GetWorldTimerManager().SetTimer(DestroyEnemyTimerHandle, this, &AEnemy::OnDestroyEnemyTimerTimeout, 1.0f, false, DestroyEnemyTime);
}

void AEnemy::OnDestroyEnemyTimerTimeout()
{
	Destroy();
}

void AEnemy::PlaySound(USoundBase* SoundToPlay)
{
	if(SoundToPlay && GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundToPlay);
	}
}

