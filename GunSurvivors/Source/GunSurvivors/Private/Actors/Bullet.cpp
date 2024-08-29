// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Bullet.h"

#include "Actors/Enemy.h"

// Sets default values
ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	SetRootComponent(SphereComponent);

	BulletSpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("BulletSprite"));
	BulletSpriteComponent->SetCollisionProfileName(TEXT("NoCollision"));
	BulletSpriteComponent->TranslucencySortPriority = 10;
	BulletSpriteComponent->SetupAttachment(RootComponent);
	
}

void ABullet::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ABullet::BeginOverlap);
}

void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!bIsLaunched)
	{
		return;
	}
	const FVector2D DistanceToMove = MovementDirection * MovementSpeed * DeltaTime;
	const FVector CurrentLocation = GetActorLocation();
	const FVector NewLocation = CurrentLocation + FVector(DistanceToMove.X, 0.f, DistanceToMove.Y);
	SetActorLocation(NewLocation);
}

void ABullet::Launch(FVector2D Direction)
{
	if(bIsLaunched)
	{
		return;
	}
	bIsLaunched = true;
	MovementDirection = Direction;
	//MovementSpeed = Speed;

	//记得定时删除
	GetWorldTimerManager().SetTimer(DeleteBulletTimer, this, &ABullet::OnDeleteTimerTimeout, 1.0f, false, DeleteTime);
}

void ABullet::OnDeleteTimerTimeout()
{
	Destroy();
}

void ABullet::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEnemy* Enemy = Cast<AEnemy>(OtherActor);
	if(Enemy && Enemy->bIsAlive)
	{
		Enemy->Die();
		//Note: 不要直接Destroy整个Bullet，因为已经有Timer了
		DisableBullet();
	}
}

void ABullet::DisableBullet()
{
	if(bIsDisabled)
	{
		return;
	}

	bIsDisabled = true;
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BulletSpriteComponent->DestroyComponent();
}

