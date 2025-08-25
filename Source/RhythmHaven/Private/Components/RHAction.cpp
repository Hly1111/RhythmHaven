// Fill out your copyright notice in the Description page of Project Settings.


#include "RHAction.h"
#include "GameFramework/Character.h"


// Sets default values for this component's properties
URHAction::URHAction() : OwnerType()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URHAction::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<ACharacter>(GetOwner());
	if (GetOwner()->ActorHasTag(FName("Enemy")))
	{
		OwnerType = EOwnerType::Enemy;
	}
	else if (GetOwner()->ActorHasTag(FName("Player")))
	{
		OwnerType = EOwnerType::Player;
	}
}


// Called every frame
void URHAction::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void URHAction::TriggerAction(FGameplayTag NewAction, EPriority)
{
	
}

