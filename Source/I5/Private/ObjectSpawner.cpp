// Fill out your copyright notice in the Description page of Project Settings.

#include "ObjectSpawner.h"
#include "Engine/World.h"

UObjectSpawner::UObjectSpawner()
{
    PrimaryComponentTick.bCanEverTick = false;
    bCanRespawn = true;
    RespawnTime = 3.0f;
}

void UObjectSpawner::BeginPlay()
{
    Super::BeginPlay();
    
    // Spawn initial actor
    if (ActorToSpawn)
    {
        SpawnActor();
    }
}

void UObjectSpawner::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

AActor* UObjectSpawner::SpawnActor()
{
    if (!ActorToSpawn)
        return nullptr;

    UWorld* World = GetWorld();
    if (!World)
        return nullptr;

    // Get the owner's transform for spawn location
    FTransform SpawnTransform = GetOwner()->GetTransform();
    
    // Spawn the actor
    SpawnedActor = World->SpawnActor<AActor>(ActorToSpawn, SpawnTransform);
    
    if (SpawnedActor)
    {
        // Bind to the destruction event
        SpawnedActor->OnDestroyed.AddDynamic(this, &UObjectSpawner::OnActorDestroyed);
        
        // Broadcast the spawn event
        OnActorSpawned.Broadcast(SpawnedActor);
    }

    return SpawnedActor;
}

void UObjectSpawner::OnActorDestroyed(AActor* DestroyedActor)
{
    if (DestroyedActor == SpawnedActor)
    {
        SpawnedActor = nullptr;
        
        // Start respawn timer if enabled
        if (bCanRespawn)
        {
            GetWorld()->GetTimerManager().SetTimer(
                RespawnTimerHandle,
                this,
                &UObjectSpawner::HandleRespawn,
                RespawnTime,
                false
            );
        }
    }
}

void UObjectSpawner::HandleRespawn()
{
    SpawnActor();
}