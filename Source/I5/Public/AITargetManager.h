// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "AITargetManager.generated.h"

class UAIPerceptionComponent;
DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(bool, FActorValidationDelegate, const AActor*, ActorToValidate);

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class I5_API UAITargetManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAITargetManager();

protected:
	UFUNCTION()
	void OnAIPerceptionUpdate(AActor* Actor, FAIStimulus Stimulus);
	UFUNCTION()
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	bool IsValidActor(AActor* ActorToValidate) const;

	//  This allows us to override the function in blueprints
	UFUNCTION(BlueprintNativeEvent, Category = "AI|Targeting")
	bool IsValidActor_Implementation(AActor* ActorToValidate) const;
UFUNCTION(BlueprintCallable, Category = "AI|Targeting")
	AActor* GetCurrentTarget() const {return this->CurrentTarget;}


private:
	UAIPerceptionComponent* PerceptionComponent;
	AActor* CurrentTarget;
		
};
