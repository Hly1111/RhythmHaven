// Fill out your copyright notice in the Description page of Project Settings.


#include "RHCharacterBase.h"
#include "Components/MotionWarping/RHMotionWarping.h"
#include "Components/RHTargeting.h"
#include "GAS/RHAbilitySystemComponent.h"
#include "GAS/RHAttributeSet.h"


// Sets default values
ARHCharacterBase::ARHCharacterBase()
{
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ASC = CreateDefaultSubobject<URHAbilitySystemComponent>(TEXT("Ability System Component"));
	AttributeSet = CreateDefaultSubobject<URHAttributeSet>(TEXT("Attribute Set"));
	TargetingComponent = CreateDefaultSubobject<URHTargeting>(TEXT("Targeting Component"));
	MotionWarpingComponent = CreateDefaultSubobject<URHMotionWarping>(TEXT("Motion Warping Component"));
}

class URHTargeting* ARHCharacterBase::GetTargetingComponent() const
{
	return TargetingComponent;
}

class URHMotionWarping* ARHCharacterBase::GetMotionWarpingComponent() const
{
	return MotionWarpingComponent;
}

UAbilitySystemComponent* ARHCharacterBase::GetAbilitySystemComponent() const
{
	return ASC;
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
	MotionWarpingComponent->InitialMotionWarpingStrategyPool();
}

