// Fill out your copyright notice in the Description page of Project Settings.


#include "Hazard.h"

// Sets default values
AHazard::AHazard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	AsteroidExplosionFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComponent"));
	AsteroidExplosionSound = CreateDefaultSubobject<UAudioComponent>(TEXT("DeathExplosionSound"));

	CollisionComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	StaticMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	AsteroidExplosionFX->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	AsteroidExplosionSound->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	Spawn_X_Velocity = -500.0f;
	Spawn_Y_Velocity = 0.0f;

	SelfDestructTimer = 1.0f;

	bHit = false;
}

void AHazard::SpawnChildren(int32 NumChildren)
{
}

void AHazard::SetHazardVelocity(FVector NewVelocity)
{
}

// Called when the game starts or when spawned
void AHazard::BeginPlay()
{
	Super::BeginPlay();
	Initial_Rotation = 0.0f;
	Initial_X_Location = 1500.0f;
	Initial_Y_Location = FMath::RandRange(-400, 400);

	RandSize = FMath::RandRange(3, 8);
	this->SetActorScale3D(FVector(RandSize, RandSize, RandSize));

	AsteroidExplosionSound->Deactivate();
	AsteroidExplosionSound->bStopWhenOwnerDestroyed = false;

	AsteroidExplosionFX->Deactivate();

	// bind the event

	OnActorBeginOverlap.AddDynamic(this, &AHazard::OnBeginOverlap);
}

// Called every frame
void AHazard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	check(this != nullptr);
	Initial_Rotation += DeltaTime;

	Initial_X_Location += DeltaTime * Spawn_X_Velocity;
	Initial_Y_Location += DeltaTime * Spawn_Y_Velocity;

	this->SetActorRotation(FRotator(Initial_Rotation * 100.0f, Initial_Rotation * 50.0f, 0.0f));
	this->SetActorLocation(FVector(Initial_X_Location, Initial_Y_Location,0.0f));

}


void AHazard::OnBeginOverlap(AActor* AsteroidActor, AActor* OtherActor)
{
}

