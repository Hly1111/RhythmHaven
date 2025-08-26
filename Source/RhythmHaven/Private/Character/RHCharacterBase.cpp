// Fill out your copyright notice in the Description page of Project Settings.


#include "RHCharacterBase.h"
#include "Components/RHHit.h"
#include "Components/RHTargeting.h"
#include "GAS/RHAbilitySystemComponent.h"
#include "GAS/RHAttributeSet.h"


// Sets default values
ARHCharacterBase::ARHCharacterBase()
{
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ASC = CreateDefaultSubobject<URHAbilitySystemComponent>(TEXT("Ability System Component"));
	AttributeSet = CreateDefaultSubobject<URHAttributeSet>(TEXT("Attribute Set"));
	HitComponent = CreateDefaultSubobject<URHHit>(TEXT("Hit Component"));
	TargettingComponent = CreateDefaultSubobject<URHTargeting>(TEXT("Targeting Component"));
}

// Called when the game starts or when spawned
void ARHCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (LinkAnimLayerClass && GetMesh())
	{
		GetMesh()->LinkAnimClassLayers(LinkAnimLayerClass);
	}

	ASC->ApplyInitialGameplayEffects();
	ASC->GiveInitialGameplayAbilities();
}

