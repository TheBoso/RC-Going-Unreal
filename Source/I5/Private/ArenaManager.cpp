// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaManager.h"
#include <ArenaMoby.h>

// Sets default values for this component's properties
UArenaManager::UArenaManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}




// Called when the game starts
void UArenaManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UArenaManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UArenaManager::StartArena()
{
    GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Arena Started!"));

    CurrentWaveIndex = 0;
    StartNextWave();
}

void UArenaManager::StartNextWave()
{
    if (SelectedArenaConfig && CurrentWaveIndex < SelectedArenaConfig->WaveEntries.Num())
    {
        SpawnWave();
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Arena complete!"));
    }
}


void UArenaManager::SpawnWave()
{
    if (!SelectedArenaConfig) return;
    this->InitialDelay = 0.0f;


    RemainingEnemies = 0;
    //  Setup the total number of enemies
    for (FArenaEnemyEntry entry : SelectedArenaConfig->WaveEntries[CurrentWaveIndex].Enemies)
    {
        RemainingEnemies += entry.EnemyCount;
    }

    GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green,
        *FString::Printf(TEXT("Enemies: %d"), RemainingEnemies));

    SpawnEnemies();
}


void UArenaManager::SpawnEnemies()
{
    if ( RemainingEnemies <= 0)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("No Remaining Enemies!"));

        GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
        return;
    }

    if (SpawnPoints.Num() == 0) return;

    for (FArenaEnemyEntry entry : SelectedArenaConfig->WaveEntries[CurrentWaveIndex].Enemies)
    {
        for (int32 i = 0; i < entry.EnemyCount; i++)
        {
            
            AActor* SpawnLocation = SpawnPoints[FMath::RandRange(0, SpawnPoints.Num() - 1)];
            if (SpawnLocation)
            {
                // Create a timer delegate with parameters
               // FTimerDelegate TimerDelegate;
             //   TimerDelegate.BindUObject(this, &UArenaManager::SpawnEnemy, entry.EnemyType);

                // Set a timer to call the SpawnEnemy function with a delay
             //   FTimerHandle TimerHandle;
            //    GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, InitialDelay, false);


              // Create a copy of the enemy type for binding
                TSubclassOf<AActor> EnemyToSpawn = entry.EnemyType;
                FVector SpawnLocationVector = SpawnLocation->GetActorLocation();
                /*

                FTimerHandle TimerHandle;
                GetWorld()->GetTimerManager().SetTimer(
                    TimerHandle,
                    [this, EnemyToSpawn, SpawnLocationVector]()
                    {
                        SpawnIndividualEnemy(EnemyToSpawn, SpawnLocationVector);
                    },
                    InitialDelay,
                    false
                );
                */

                  SpawnIndividualEnemy(entry.EnemyType, SpawnLocationVector);

              //  InitialDelay += SpawnInterval; // Increase delay for next enemy




           

            }
        }
    }
}

void UArenaManager::SpawnIndividualEnemy(TSubclassOf<AActor> enemy, FVector spawnLocation)
{
    float XOffset = FMath::RandRange(-5.0f, 5.0f); // Adjust the range as needed
    float YOffset = FMath::RandRange(-5.0f, 5.0f); // Adjust the range as needed
    spawnLocation.X += XOffset;
    spawnLocation.Y += YOffset;

    AActor* SpawnedEnemy = GetWorld()->SpawnActor<AActor>(enemy, spawnLocation, FRotator::ZeroRotator);
    if (SpawnedEnemy)
    {
        SpawnedEnemy->OnDestroyed.AddDynamic(this, &UArenaManager::OnEnemyDestroyed);
        UArenaMoby* arenaMoby = SpawnedEnemy->FindComponentByClass < UArenaMoby>();
        if (arenaMoby != nullptr)
        {
            arenaMoby->OnArenaInit();
        }
    }

}


void UArenaManager::OnEnemyDestroyed(AActor* DestroyedEnemy)
{
    RemainingEnemies--;
    GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green,
        *FString::Printf(TEXT("Arena Enemy Killed! Remaining: %d"), RemainingEnemies));


    if (RemainingEnemies <= 0)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("All enemies killed, starting new wave!"));

        CurrentWaveIndex++;
        StartNextWave();
    }
}


