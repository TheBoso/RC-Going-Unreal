// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ArenaMoby.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class I5_API UArenaMoby : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UArenaMoby();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Declare a function that will be implemented in Blueprints
	UFUNCTION(BlueprintImplementableEvent, Category = "Custom")
	void OnArenaInit_Implementation();

	void OnArenaInit();
		
};
