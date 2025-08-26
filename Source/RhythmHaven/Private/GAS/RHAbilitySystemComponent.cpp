// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/RHAbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Character/EnemyCharacter/RHEnemyCharacter.h"
#include "GameFramework/Character.h"
#include "GAS/RHGameplayAbilityBase.h"
#include "Kismet/KismetSystemLibrary.h"

void URHAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();
	Owner = Cast<ACharacter>(GetOwner());
	if (Owner->ActorHasTag(FName("Player")))
	{
		OwnerType = EOwnerType::Player;
	}
	else if (Owner->ActorHasTag(FName("Enemy")))
	{
		OwnerType = EOwnerType::Enemy;
	}
}

void URHAbilitySystemComponent::ApplyInitialGameplayEffects()
{
	for (auto& Effects:InitialGameEffects)
	{
		FGameplayEffectSpecHandle EffectHandle = MakeOutgoingSpec(Effects, 1, MakeEffectContext());
		ApplyGameplayEffectSpecToSelf(*EffectHandle.Data.Get());
	}
}

void URHAbilitySystemComponent::GiveInitialGameplayAbilities()
{
	for (auto AbilityClass:InitialAbilities)
	{
		GiveAbility(FGameplayAbilitySpec(AbilityClass, 1, -1, nullptr));
	}
}

void URHAbilitySystemComponent::TryActivateCombo(FGameplayTag ComboTag)
{
	TryActivateAbilitiesByTag(FGameplayTagContainer(ComboTag));

	if (PreInputEffect)
	{
		FGameplayEffectSpecHandle Handle = MakeOutgoingSpec(PreInputEffect, 1, MakeEffectContext());
		ApplyGameplayEffectSpecToSelf(*Handle.Data.Get());
	}
}
