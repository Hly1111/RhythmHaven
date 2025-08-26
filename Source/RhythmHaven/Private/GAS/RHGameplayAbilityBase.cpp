// Fill out your copyright notice in the Description page of Project Settings.


#include "RHGameplayAbilityBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

void URHGameplayAbilityBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (!K2_CommitAbility())
	{
		K2_EndAbility();
		return;
	}

	if (AnimMontage)
	{
		UAbilityTask_PlayMontageAndWait* MontageAndWait = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, AnimMontage);
		MontageAndWait->OnCompleted.AddDynamic(this, &URHGameplayAbilityBase::K2_EndAbility);
		MontageAndWait->OnInterrupted.AddDynamic(this, &URHGameplayAbilityBase::K2_EndAbility);
		MontageAndWait->ReadyForActivation();
	}
}

void URHGameplayAbilityBase::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
