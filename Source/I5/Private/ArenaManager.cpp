// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaManager.h"

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

    GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Spawning Wave"));

    //  Setup the total number of enemies
    for (FArenaEnemyEntry entry : SelectedArenaConfig->WaveEntries[CurrentWaveIndex].Enemies)
    {
        RemainingEnemies += entry.EnemyCount;
    }

    SpawnEnemy();
}


void UArenaManager::SpawnEnemy()
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
                FVector SpawnLocationVector = SpawnLocation->GetActorLocation();
                AActor* SpawnedEnemy = GetWorld()->SpawnActor<AActor>(entry.EnemyType, SpawnLocationVector, FRotator::ZeroRotator);
                //  Hack: Move the enemy slightly so that it can fire overlap events
                //SpawnedEnemy->SetActorLocation(SpawnLocationVector * 2);

                /*
                FVector ForwardOffset = SpawnedEnemy->GetActorForwardVector() * 1.0f; // Adjust the value as need
                */


                GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Enemy Spawned"));

                if (SpawnedEnemy)
                {
                    SpawnedEnemy->OnDestroyed.AddDynamic(this, &UArenaManager::OnEnemyDestroyed);
                }

            }
        }
    }
}


void UArenaManager::OnEnemyDestroyed(AActor* DestroyedEnemy)
{
    RemainingEnemies--;
    if (RemainingEnemies <= 0)
    {
        CurrentWaveIndex++;
        StartNextWave();
    }
}


