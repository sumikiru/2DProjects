// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/EnemySpawner.h"

#include "Kismet/GameplayStatics.h"

AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GetWorld());
	if (GameMode)
	{
		MyGameMode = Cast<AGunSurvivorsGameMode>(GameMode);
	}

	//GetActorOfClass开销较大，与其每次在Enemy的BeginPlay中执行，不如直接在EnemySpawner的zBeginPlay中生成一次即可。
	AActor* PlayerActor = UGameplayStatics::GetActorOfClass(GetWorld(), ATopDownCharacter::StaticClass());
	//方法二：直接按照Tag查找
	// TArray<AActor*> PlayerActors;
	// UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ATopDownCharacter::StaticClass(), TEXT("Player"), PlayerActors);
	// PlayerActor = PlayerActors[0];
	if(PlayerActor)
	{
		Player = Cast<ATopDownCharacter>(PlayerActor);
		if(Player->ActorHasTag(TEXT("Player")))
		{
			Player->PlayerDieDelegate.AddDynamic(this, &AEnemySpawner::OnPlayerDied);
			StartSpawning();
		}
	}
}

void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemySpawner::SetupEnemy(AEnemy* Enemy)
{
	if(Enemy)
	{
		Enemy->TargetPlayer = Player;
		Enemy->bCanFollowPlayer = true;
		Enemy->EnemyDieDelegate.AddDynamic(this, &AEnemySpawner::OnEnemyDied);//将OnEnemyDie函数绑定到EnemyDieDelegate委托中
		Enemy->AttackDamage = AttackDamage;
		
		TotalEnemyCount += 1;//只是对单个SpawnPoint的统计
		if(TotalEnemyCount % DifficultySpawnCountLevel == 0)
		{
			SpawnDuration = FMath::Clamp(SpawnDuration - DecreaseSpawnDurationByEachLevel, MinSpawnDuration, SpawnDuration);

			//Note: 为了使用新的生成时间，重新SetTimer(SpawnDuration已经减少了)
			EndSpawning();
			StartSpawning();
		}
	}
}

void AEnemySpawner::SpawnEnemy()
{
	UWorld* World = GetWorld();
	if(World)
	{
		FVector2D RandomLocation = FVector2D(FMath::VRand());
		RandomLocation = RandomLocation.GetSafeNormal();
		RandomLocation *= FMath::RandRange(MinSpawnDistance, MaxSpawnDistance);
		const FVector EnemySpawnLocation = GetActorLocation() + FVector(RandomLocation.X, 0.f, RandomLocation.Y);

		//Increase Difficulty
		AEnemy* Enemy = World->SpawnActor<AEnemy>(EnemyToSpawnClass, EnemySpawnLocation, FRotator::ZeroRotator);
		SetupEnemy(Enemy);
	}
}

void AEnemySpawner::StartSpawning()
{
	GetWorldTimerManager().SetTimer(SpawnEnemyTimer, this, &AEnemySpawner::OnSpawnTimerTimeout, SpawnDuration, true, SpawnDuration);
}

void AEnemySpawner::EndSpawning()
{
	GetWorldTimerManager().ClearTimer(SpawnEnemyTimer);
}

void AEnemySpawner::OnSpawnTimerTimeout()
{
	SpawnEnemy();
}

void AEnemySpawner::OnEnemyDied()
{
	Player->ScoreChangedDelegate.Broadcast(ScoreToAdd);
	//上面一行代码等同于下面的注释部分
	// AGameHUD* GameHUD = Cast<AGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	// if(Player && Player->Attribute && GameHUD && GameHUD->GetGunSurvivorsOverlay())
	// {
	// 	Player->Attribute->AddScore(ScoreToAdd);
	// 	GameHUD->GetGunSurvivorsOverlay()->SetScoreText(Player->Attribute->GetScore());
	// }
}

void AEnemySpawner::OnPlayerDied()
{
	EndSpawning();

	TArray<AActor*>EnemyArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), EnemyArray);
	for(AActor* EnemyActor : EnemyArray)
	{
		AEnemy* Enemy = Cast<AEnemy>(EnemyActor);
		if(Enemy && Enemy->bIsAlive)
		{
			Enemy->bCanFollowPlayer = false;
		}
	}

	MyGameMode->RestartGame();
}

