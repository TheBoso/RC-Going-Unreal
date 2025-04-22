// Fill out your copyright notice in the Description page of Project Settings.


#include "AITargetManager.h"

#include "Perception/AIPerceptionComponent.h"

// Sets default values for this component's properties
UAITargetManager::UAITargetManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UAITargetManager::OnAIPerceptionUpdate(AActor* Actor, FAIStimulus Stimulus)
{
    // First check if the detected actor is valid
    if (!IsValidActor(Actor))
    {
    //	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Not enemy, returning."));

        return;
    }

    // If we don't have a current target, set this one as our target
    if (!CurrentTarget)
    {
        CurrentTarget = Actor;
        return;
    }

    // If this is our current target
    if (Actor == CurrentTarget)
    {
        // If we lost sight of our current target (Stimulus.WasSuccessfullySensed() is false)
        if (!Stimulus.WasSuccessfullySensed())
        {
            CurrentTarget = nullptr;
        //    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Lost track of target"));
        }
        return;
    }

    // At this point, we have a current target and detected a different actor
    // You might want to implement additional logic here, such as:
    // - Switch to closer targets
    // - Switch to targets with higher priority
    // For now, we'll keep our current target
    
    // Example of switching to closer targets:
    if (GetOwner())
    {
    	FVector location = GetOwner()->GetActorLocation();
    	
        float DistanceToCurrentTarget = FVector::Distance(location, CurrentTarget->GetActorLocation());
        float DistanceToNewTarget = FVector::Distance(location, Actor->GetActorLocation());

        if (DistanceToNewTarget < DistanceToCurrentTarget)
        {
            CurrentTarget = Actor;
        //    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Switching to closer target"));
        }
    }
}


// Called when the game starts
void UAITargetManager::BeginPlay()
{
	Super::BeginPlay();
	this->PerceptionComponent = GetOwner()->FindComponentByClass<UAIPerceptionComponent>();
	if (this->PerceptionComponent == nullptr)
	{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Failed to find AI Perception Component"));
		return;
	}

	this->PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &UAITargetManager::OnAIPerceptionUpdate);


	
}


// Called every frame
void UAITargetManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UAITargetManager::IsValidActor(AActor* ActorToValidate) const
{
	if (ActorToValidate == nullptr) return false;
	if (ActorToValidate->IsPendingKill()) return false;

	return IsValidActor_Implementation(ActorToValidate);
}

bool UAITargetManager::IsValidActor_Implementation_Implementation(AActor* ActorToValidate) const
{
	return true;
}
