// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ObjectSpawner.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnObjectSpawn, AActor*, SpawnedActor);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class I5_API UObjectSpawner : public UActorComponent
{
    GENERATED_BODY()

public:    
    UObjectSpawner();

protected:
    virtual void BeginPlay() override;

public:    
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // The actor class to spawn
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
    TSubclassOf<AActor> ActorToSpawn;

    // Whether the actor can respawn after being destroyed
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
    bool bCanRespawn;

    // Time in seconds to wait before respawning
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner", meta = (EditCondition = "bCanRespawn"))
    float RespawnTime;

    // Event dispatcher for when an actor is spawned
    UPROPERTY(BlueprintAssignable, Category = "Spawner")
    FOnObjectSpawn OnActorSpawned;

private:
    // Handle to manage the respawn timer
    FTimerHandle RespawnTimerHandle;
    
    // Pointer to keep track of the spawned actor
    UPROPERTY()
    AActor* SpawnedActor;

    // Internal function to spawn the actor
    AActor* SpawnActor();

    // Called when the spawned actor is destroyed
    UFUNCTION()
    void OnActorDestroyed(AActor* DestroyedActor);

    // Internal function to handle respawning
    UFUNCTION()
    void HandleRespawn();
};