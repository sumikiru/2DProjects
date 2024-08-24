// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Obstacle.h"

#include "Kismet/GameplayStatics.h"
#include "Pawns/PlayerCharacter.h"

AObstacle::AObstacle()
{
	PrimaryActorTick.bCanEverTick = true;

	/* Components */
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	//CapsuleComponent->SetCollisionProfileName(TEXT("NoCollision"));
	RootComponent = CapsuleComponent;

	ObstacleSpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("ObstacleSpriteComponent"));
	ObstacleSpriteComponent->SetCollisionProfileName(TEXT("NoCollision"));
	ObstacleSpriteComponent->SetupAttachment(RootComponent);
	ObstacleSpriteComponent->SetTranslucentSortPriority(4);
}

void AObstacle::BeginPlay()
{
	Super::BeginPlay();

	CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &AObstacle::BeginOverlap);
	//CapsuleComponent->OnComponentEndOverlap.AddDynamic(this, &AObstacle::EndOverlap);

	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GetWorld());
	MyGameMode = Cast<ADesertRacerGameMode>(GameMode);
}

void AObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AObstacle::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	if(Player && Player->ActorHasTag(TEXT("Player")))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("%s Hit Obstacle!"), *OtherActor->GetName()));
		if(Player->bCanMove)
		{
			if(!bIsStartLine && !bIsTire)
			{	
				Player->bCanMove = false;
				UGameplayStatics::PlaySound2D(GetWorld(), HitSound);//3D则使用PlaySoundAtLocation
				MyGameMode->ResetGameLevel(bIsFinishLine);
			}
			else if(bIsTire)
			{
				const FVector PushDirection = Player->GetActorUpVector();
				const float PushSpeed = 500.f;
				//const float DeltaTime = GetWorld()->DeltaTimeSeconds;
				this->SetActorLocation(this->GetActorLocation() + PushDirection * PushSpeed * 0.1f);
			}
			else if(bIsStartLine)
			{
				this->Destroy();
			}
		}
	}
}

