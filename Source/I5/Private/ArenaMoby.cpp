// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaMoby.h"

// Sets default values for this component's properties
UArenaMoby::UArenaMoby()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UArenaMoby::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UArenaMoby::OnArenaInit()
{
	this->OnArenaInit_Implementation();
}


