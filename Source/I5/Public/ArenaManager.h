// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ArenaManager.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class I5_API UArenaManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UArenaManager();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arena")
	UArenaConfig* SelectedArenaConfig;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arena")
	TArray<AActor*> SpawnPoints;

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Arena")
	void StartArena();
	void StartNextWave();
	void SpawnWave();
	void OnEnemyDestroyed(AActor* DestroyedEnemy);

private:
	int32 CurrentWaveIndex;
	int32 RemainingEnemies;
	FTimerHandle SpawnTimerHandle;
	void SpawnEnemy();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};



USTRUCT(BlueprintType)
struct FArenaEnemyEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	TSubclassOf<AActor> EnemyType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	int32 EnemyCount;
};



USTRUCT(BlueprintType)
struct FArenaWaveEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	TArray<FArenaEnemyEntry> Enemies;
};


UCLASS()
class I5_API UArenaConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arena")
	TArray<FArenaWaveEntry> WaveEntries;
};




