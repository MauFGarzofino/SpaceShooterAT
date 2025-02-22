// Fill out your copyright notice in the Description page of Project Settings.


#include "SS_Player.h"

// Sets default values
ASS_Player::ASS_Player()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	ParticleSystems = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComponent"));
	ExplosionFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ExplosionFX"));
	DeathExplosionSound = CreateDefaultSubobject<UAudioComponent>(TEXT("DeathExplosionSound"));

	CollisionComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ShipMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ParticleSystems->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ExplosionFX->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	DeathExplosionSound->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	MaxVelocity = 250.0f;
	Current_X_Velocity = 0.0f;
	Current_Y_Velocity = 0.0f;

	bIsFiring = false;
	WeaponFireRate = 0.25f;
	TimeSinceLastShot = 0.0f;

	PlayerScore = 0.0f;
}

void ASS_Player::MoveRigth(float AxisValue)
{
	Current_X_Velocity = MaxVelocity * AxisValue;
}

void ASS_Player::MoveUp(float AxisValue)
{
	Current_Y_Velocity = MaxVelocity * AxisValue;
}

void ASS_Player::StarFiring()
{
	bIsFiring = true;
}

void ASS_Player::StopFiring()
{
	bIsFiring = false;
}

void ASS_Player::FireWeapon()
{
	FActorSpawnParameters Params = {};
	Params.Owner = this;

	AActor* SpawnProjectile = GetWorld()->SpawnActor(WeaponProjectile_BP, &Current_Location, &Current_Rotation, Params);

}

void ASS_Player::OnBeginOverLap(AActor* PlayerActor, AActor* OtherActor)
{
}

void ASS_Player::CollectablePickup()
{
}

// Called when the game starts or when spawned
void ASS_Player::BeginPlay()
{
	Super::BeginPlay();
	Current_Location = this->GetActorLocation();
	Current_Rotation = this->GetActorRotation();

	bHit = false;
	bDead = false;

	ExplosionFX->Deactivate();
	DeathExplosionSound->Deactivate();

	Max_Healt = 100.0f;
	Current_Healt = 100.0f;

	Max_Armor = 100.0f;
	Current_Armor = 100.0f;

	OnActorBeginOverlap.AddDynamic(this,&ASS_Player::OnBeginOverLap);

	Field_Width = 700.f;
    Field_Height = 400.f;

}

// Called every frame
void ASS_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Current_X_Velocity != 0.0f || Current_Y_Velocity != 0.0f) {

		New_Location = FVector(Current_Location.X + (Current_X_Velocity * DeltaTime),
			Current_Location.Y + (Current_Y_Velocity * DeltaTime), 0);

		this->SetActorLocation(New_Location);

		Current_Location = New_Location;

	}

	if (Current_Y_Velocity > 0.01f) {

		this->SetActorRotation(FRotator(Current_Rotation.Pitch + 45.0f, Current_Rotation.Yaw,
			Current_Rotation.Roll));
	}
	else if (Current_Y_Velocity < -0.1f) {
		this->SetActorRotation(FRotator(Current_Rotation.Pitch - 45.0f, Current_Rotation.Yaw,
			Current_Rotation.Roll));
	}
	else {
		this->SetActorRotation(FRotator(Current_Rotation));
	}

	if (this->GetActorLocation().X > Field_Width) 
		Current_Location = FVector(Field_Width -1,Current_Location.Y, Current_Location.Z);

	if(this->GetActorLocation().X < -Field_Width)
		Current_Location = FVector(- Field_Width + 1, Current_Location.Y, Current_Location.Z);
	
	if(this->GetActorLocation().Y >Field_Height)
		Current_Location = FVector(Current_Location.X, Field_Height -1, Current_Location.Z);

	if (this->GetActorLocation().Y < -Field_Height)
		Current_Location = FVector(Current_Location.X, -Field_Height + 1, Current_Location.Z);

	//handle firing
	if (bIsFiring) {
		if (TimeSinceLastShot > WeaponFireRate) {
			FireWeapon();
			TimeSinceLastShot = 0.0f;
	    }
	}
	TimeSinceLastShot += DeltaTime;
	// this means:
	//TimeSinceLastShot = TimeSinceLastShot + DeltaTime;
}

// Called to bind functionality to input
void ASS_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(FName("MoveRigth"), this, &ASS_Player::MoveRigth);
	PlayerInputComponent->BindAxis(FName("MoveUp"), this, &ASS_Player::MoveUp);
	PlayerInputComponent->BindAction(FName("Fire"), IE_Pressed,this, &ASS_Player::StarFiring);
	PlayerInputComponent->BindAction(FName("Fire"), IE_Released, this, &ASS_Player::StopFiring);
}

