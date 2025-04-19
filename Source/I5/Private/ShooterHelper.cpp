// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterHelper.h"

#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values for this component's properties
UShooterHelper::UShooterHelper()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UShooterHelper::BeginPlay()
{
	Super::BeginPlay();
CachedMesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
	// ...
	
}


// Called every frame
void UShooterHelper::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UShooterHelper::SpawnAndShootBullet()
{
	if (!BulletClass || CachedMesh == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("BulletClass or WeaponMesh not set in ShooterHelper"));
		return;
	}

	// Get the socket transform
	FTransform SocketTransform = CachedMesh->GetSocketTransform(SocketName);
	FVector SpawnLocation = SocketTransform.GetLocation();
	FRotator SpawnRotation = SocketTransform.Rotator();

	// Get the world
	UWorld* World = GetWorld();
	if (!World) return;

	// Spawn parameters
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	SpawnParams.Instigator = Cast<APawn>(GetOwner());

	// Spawn the bullet
	AActor* Bullet = World->SpawnActor<AActor>(BulletClass, SpawnLocation, SpawnRotation, SpawnParams);
	if (Bullet)
	{
		// Broadcast the delegate with the spawned bullet
		OnBulletSpawned.Broadcast(Bullet);

		
		// Get the projectile movement component
		UProjectileMovementComponent* ProjectileMovement = Bullet->FindComponentByClass<UProjectileMovementComponent>();
		if (ProjectileMovement)
		{
			// Initialize the projectile's velocity
			ProjectileMovement->InitialSpeed = BulletSpeed;
			ProjectileMovement->MaxSpeed = BulletSpeed;
			ProjectileMovement->Velocity = SpawnRotation.Vector() * BulletSpeed;
		}
		else
		{
			// If there's no ProjectileMovementComponent, we can apply velocity to the root component
			UStaticMeshComponent* MeshComponent = Bullet->FindComponentByClass<UStaticMeshComponent>();
			if (MeshComponent)
			{
				MeshComponent->SetPhysicsLinearVelocity(SpawnRotation.Vector() * BulletSpeed);
			}
		}
	}
}

