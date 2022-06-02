// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Projectile.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision Component"));
	ExplosionFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Explosion FX"));
	ExplosionSound = CreateDefaultSubobject<UAudioComponent>(TEXT("Explosion Sound"));

	CollisionComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	StaticMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ExplosionFX->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ExplosionSound->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	fBurstDelay = 0.15f;
}


// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	TotalTime = 0.0f;
	TimeSinceLastShot = 0.0f;

	bHit = false;
	bDestroy = false;

	fDestroyTimer = 1.0f;
     
	ExplosionFX->Deactivate();
	ExplosionSound->Deactivate();

	ThisWorld = GetWorld();

	// return a random integer between 0 and RAND_MAX
	RandomStart = FMath::Rand();

	OnActorBeginOverlap.AddDynamic(this, &AEnemy::OnBeginOverlap);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TotalTime += DeltaTime;
	TimeSinceLastShot += DeltaTime;

	CurrentLocation = this->GetActorLocation();
	CurrentRotation = this->GetActorRotation();

	CurrentLocation.Y += FMath::Sin(TotalTime + RandomStart);

	this->SetActorLocation(CurrentLocation - (CurrentVelocity * DeltaTime));

	// Handle Shooting
	if (TimeSinceLastShot >= 1.0f && !bHit) {

		if (fBurstDelay >= 0.01f) {

			FActorSpawnParameters Params = {};
			Params.Owner = this;

			GetWorld()->SpawnActor(EnemyProjectile, &CurrentLocation, &CurrentRotation, Params);

			fBurstDelay = 0.0f;

		}
		TimeSinceLastShot = 0.0f;
		fBurstDelay += DeltaTime;
	}

	// Handle Destroying
	if (bDestroy) {
		this->Destroy();
	}

	if (bHit) {
		StaticMesh->SetVisibility(false);
		this->SetActorEnableCollision(false);
		ExplosionFX->Activate();
		ExplosionSound->Activate();

		fDestroyTimer -= DeltaTime;
	}

	if (fDestroyTimer <= 0.0f) {
		this->Destroy();
	}
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::OnBeginOverlap(AActor* EnemyActor, AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Bounds")) {
		bDestroy = true;
	}

	if (OtherActor->ActorHasTag("Asteroid")|| OtherActor->ActorHasTag("Player")) {
		bHit = true;
	}

	if (OtherActor->ActorHasTag("Projectile")) {
		
		if (ThisWorld) {
			bHit = true;
            //spawn pickup
		}
	}
}
