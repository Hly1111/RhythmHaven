// Fill out your copyright notice in the Description page of Project Settings.


#include "RHCharacterBase.h"
#include "Components/RHAction.h"
#include "Components/RHHit.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interface/RHCharacterDataInterface.h"


// Sets default values
ARHCharacterBase::ARHCharacterBase()
{
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ActionComponent = CreateDefaultSubobject<URHAction>(TEXT("Action Component"));
	HitComponent = CreateDefaultSubobject<URHHit>(TEXT("Hit Component"));
}

// Called when the game starts or when spawned
void ARHCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (LinkAnimLayerClass && GetMesh())
	{
		GetMesh()->LinkAnimClassLayers(LinkAnimLayerClass);
	}
}

