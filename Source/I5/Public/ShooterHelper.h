// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShooterHelper.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBulletSpawned, AActor*, SpawnedBullet);
UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class I5_API UShooterHelper : public UActorComponent
{
    GENERATED_BODY()

public:    
    // Sets default values for this component's properties
    UShooterHelper();

protected:
    // Called when the game starts
    virtual void BeginPlay() override;
    USkeletalMeshComponent* CachedMesh;

public:

    // Delegate that fires when a bullet is spawned
    UPROPERTY(BlueprintAssignable, Category = "Shooting")
    FOnBulletSpawned OnBulletSpawned;


    
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    
    // Bullet class to spawn
    UPROPERTY(EditAnywhere, Category = "Shooting")
    TSubclassOf<AActor> BulletClass;
    
    // Bullet speed
    UPROPERTY(EditAnywhere, Category = "Shooting")
    float BulletSpeed = 1500.0f;


        
    // Name of the socket to spawn bullets from
    UPROPERTY(EditAnywhere, Category = "Shooting")
    FName SocketName = FName("MuzzleSocket");

    UPROPERTY(EditAnywhere, Category = "Shooting")
    bool LookAtTarget;


    
    
    // Function to spawn and shoot bullet
    UFUNCTION(BlueprintCallable, Category = "Shooting")
    void SpawnAndShootBullet();
    
};