// Fill out your copyright notice in the Description page of Project Settings.


#include "SS_GameMode.h"

ASS_GameMode::ASS_GameMode()
{
	SpawnTimer = 0.0f;

	MusicTrack = CreateAbstractDefaultSubobject<UAudioComponent>(TEXT("Music"));
	MusicTrack->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	AsteroidSpawnLoc = FVector(1000.0f,1000.0f,1000.0f);
	AsteroidSpawnRot = FRotator(0.0f,0.0f,0.0f);

	ThisWorld = GetWorld();
}

void ASS_GameMode::BeginPlay()
{

	Super::BeginPlay();
	//MusicTrack->Play();
	//bPlayerDead = false;
}

void ASS_GameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SpawnTimer = FMath::RandRange(0, 1000);
	if (Enemy) {

		if (SpawnTimer > 999) {

			FVector EnemySpawnLoc(0.0f, 0.0f, 0.0f);
			FRotator EnemySpawnRot(0.0f, 0.0f, 0.0f);

			AActor* NewEnemy = GetWorld()-> SpawnActor(Enemy, &EnemySpawnLoc, &EnemySpawnRot, AsteroidSpawnParameters);

			if (NewEnemy) 
			   NewEnemy->SetActorLocation(FVector(600.0f, FMath::RandRange(-400,400), 0.0f));
			  
		}
	}

	if (HazardTemplate) {

		if (SpawnTimer > 999) {

			AActor* NewHazard = GetWorld()->SpawnActor(HazardTemplate, &AsteroidSpawnLoc, &AsteroidSpawnRot, AsteroidSpawnParameters);


			if (NewHazard)
				NewHazard->SetActorLocation(FVector(1000.0f, 1000.0f, 1000.0f));
		}

	}

} // Spawn enemy

TSubclassOf<AHazard> ASS_GameMode::GetHazardTemplate()
{
	return TSubclassOf<AHazard>();
}

void ASS_GameMode::RestarLevel(FName LevelName)
{
}
