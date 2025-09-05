// Fill out your copyright notice in the Description page of Project Settings.


#include "RHDodgeAbility.h"

#include "AbilitySystemComponent.h"
#include "GameplayTagsManager.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Character/RHCharacterBase.h"

void URHDodgeAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                      const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!K2_CommitAbility())
	{
		K2_EndAbility();
	}

	Owner = Cast<ACharacter>(ActorInfo->AvatarActor);
	ELocomotionDirection CurrentDirection = IRHCharacterDataInterface::Execute_GetDirection(Owner);
	auto* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, DodgeMontages[CurrentDirection]);
	PlayMontageTask->OnCompleted.AddDynamic(this, &URHDodgeAbility::K2_EndAbility);
	PlayMontageTask->ReadyForActivation();
	ActorInfo->AbilitySystemComponent->AddLooseGameplayTag(UGameplayTagsManager::Get().RequestGameplayTag(FName("Attack.Dodge")));
}

void URHDodgeAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ActorInfo->AbilitySystemComponent->RemoveLooseGameplayTag(UGameplayTagsManager::Get().RequestGameplayTag(FName("Attack.Dodge")));
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
