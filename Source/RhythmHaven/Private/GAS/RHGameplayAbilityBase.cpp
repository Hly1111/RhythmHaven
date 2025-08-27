// Fill out your copyright notice in the Description page of Project Settings.


#include "RHGameplayAbilityBase.h"
#include "GameplayAbilityData.h"
#include "RHAbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Character/RHCharacterBase.h"
#include "Components/RHTargeting.h"
#include "Components/MotionWarping/MotionWarpingStrategyBase.h"
#include "Components/MotionWarping/RHMotionWarping.h"

void URHGameplayAbilityBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                             const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                             const FGameplayEventData* TriggerEventData)
{
	if (!K2_CommitAbility())
	{
		K2_EndAbility();
		return;
	}

	AbilityOwner = Cast<ARHCharacterBase>(ActorInfo->AvatarActor);
	if (!IsValid(AbilityOwner)) return;

	TryMotionWarping();
	TryPlayMontage();
}

void URHGameplayAbilityBase::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void URHGameplayAbilityBase::TryMotionWarping()
{
	if (!ActionData->bUseMotionWarping) return;
	if (!AbilityOwner->ActorHasTag("Player")) return;
	AActor* EnemyRef = AbilityOwner->GetTargetingComponent()->GetCurrentEnemy();
	UMotionWarpingStrategyBase* WarpingStrategy = AbilityOwner->GetMotionWarpingComponent()->GetStrategy(ActionData->MotionWarpingType);
	WarpingStrategy->ApplyWarp(*ActionData, AbilityOwner, EnemyRef, WarpingLocation, WarpingRotation);
	if (ActionData->MotionWarpingType == EMotionWarpingType::OnGroundAttack)
	{
		WarpingLocationFinal = WarpingLocation;
		WarpingRotationFinal = WarpingRotation;
		AbilityOwner->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocationAndRotation(FName("OnGroundCombo"), WarpingLocationFinal, WarpingRotationFinal);
	}
}

void URHGameplayAbilityBase::TryPlayMontage()
{
	if (ActionData->AnimMontage)
	{
		UAbilityTask_PlayMontageAndWait* MontageAndWait = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, ActionData->AnimMontage);
		MontageAndWait->OnCompleted.AddDynamic(this, &URHGameplayAbilityBase::K2_EndAbility);
		MontageAndWait->OnInterrupted.AddDynamic(this, &URHGameplayAbilityBase::K2_EndAbility);
		MontageAndWait->ReadyForActivation();
	}
}
