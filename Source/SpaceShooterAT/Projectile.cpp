// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	BulletComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlaneComponent"));
	CapsuleCollsiion = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComponent"));

	BulletComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	CapsuleCollsiion->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	ProjectileSpeed = 1000.0f;
    
	bHit = false;
	bDestroy = false; 
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentLocation = this->GetActorLocation();

	OnActorBeginOverlap.AddDynamic(this, &AProjectile::OnBeginOverlap);
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewLocation = FVector(CurrentLocation.X + (ProjectileSpeed * DeltaTime),
		CurrentLocation.Y, CurrentLocation.Z);

	this->SetActorLocation(NewLocation);
	CurrentLocation = NewLocation;

}

void AProjectile::OnBeginOverlap(AActor* ProjectileActor, AActor* OtherActor)
{
}
