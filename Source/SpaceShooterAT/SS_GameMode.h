// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Enemy.h"
#include "Hazard.h"
#include "SS_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class SPACESHOOTERAT_API ASS_GameMode : public AGameMode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	    TSubclassOf<AEnemy> Enemy;
	UPROPERTY(EditAnywhere)
	    TSubclassOf<AHazard> HazardTemplate;

	UPROPERTY(EditAnywhere)
		UAudioComponent* MusicTrack;
	
	ASS_GameMode();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	TSubclassOf<AHazard> GetHazardTemplate();

public:
	int32 SpawnTimer;
	bool bPlayerDead;

	UFUNCTION()
		void RestarLevel(FName LevelName);
	bool bStart;

protected:
	FActorSpawnParameters AsteroidSpawnParameters;
	FVector AsteroidSpawnLoc;
	FRotator AsteroidSpawnRot;

	UWorld* ThisWorld;

	//add widgets
};
