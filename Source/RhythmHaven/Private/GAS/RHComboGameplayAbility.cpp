// Fill out your copyright notice in the Description page of Project Settings.


#include "RHComboGameplayAbility.h"
#include "GameplayAbilityData.h"
#include "GameplayTagsManager.h"
#include "RHAbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Character/RHCharacterBase.h"
#include "Components/RHTargeting.h"
#include "Components/WeaponHitBox.h"
#include "Components/MotionWarping/MotionWarpingData.h"
#include "Components/MotionWarping/MotionWarpingStrategyBase.h"
#include "Components/MotionWarping/RHMotionWarping.h"

void URHComboGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                             const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                             const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (!K2_CommitAbility())
	{
		K2_EndAbility();
		return;
	}

	AbilityOwner = Cast<ARHCharacterBase>(ActorInfo->AvatarActor);
	if (!IsValid(AbilityOwner)) return;

	CurrentHitParams = FRHHitParams
	{
		ActionData->GameplayEffectToApply,
		ActionData->Damage,
		ActionData->EnemyHitSound,
		ActionData->EnemyHitMontage,
		ActionData->EnemyUseMotionWarping,
		ActionData->EnemyMotionWarpingType,
		ActionData->EnemyHitBackDistance,
		ActionData->EnemyWarpingOffsetX,
		ActionData->EnemyWarpingOffsetY,
		ActionData->EnemyWarpingOffsetZ,
		ActionData->EnemyMovementModeWhenHit,
		ActionData->EnemyInterpSpeed,
		ActionData->EnemyInterpTimeLength,
		ActionData->bPlayCameraShake,
		ActionData->CameraShakeType
	};
	TryHitBoxSetUp();
	
	TryMotionWarping();
	TryPlayMontage();
}

void URHComboGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	SendGameplayEvent(UGameplayTagsManager::Get().RequestGameplayTag("Event.DisableHitBox"), FGameplayEventData());
}

void URHComboGameplayAbility::TryHitBoxSetUp()
{
	auto* OpenTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, UGameplayTagsManager::Get().RequestGameplayTag("Event.EnableHitBox"));
	OpenTask->EventReceived.AddDynamic(this, &URHComboGameplayAbility::OnHitWindowOpen);
	OpenTask->ReadyForActivation();

	auto* CloseTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, UGameplayTagsManager::Get().RequestGameplayTag("Event.DisableHitBox"));
	CloseTask->EventReceived.AddDynamic(this, &URHComboGameplayAbility::OnHitWindowClose);
	CloseTask->ReadyForActivation();

	auto* PlayAttackSoundTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, UGameplayTagsManager::Get().RequestGameplayTag("Event.PlayAttackSound"));
	PlayAttackSoundTask->EventReceived.AddDynamic(this, &URHComboGameplayAbility::PlayAttackSound);
	PlayAttackSoundTask->ReadyForActivation();
}

void URHComboGameplayAbility::TryMotionWarping()
{
	if (!ActionData->bUseMotionWarping) return;
	if (!AbilityOwner->ActorHasTag("Player")) return;
	AActor* EnemyRef = AbilityOwner->GetTargetingComponent()->GetCurrentEnemy();
	UMotionWarpingStrategyBase* WarpingStrategy = AbilityOwner->GetMotionWarpingComponent()->GetStrategy(ActionData->MotionWarpingType);
	FMotionWarpingData WarpingData = FMotionWarpingData
	{
		ActionData->PlayerWarpTriggerRange,
		ActionData->PlayerWarpingOffsetX,
		ActionData->PlayerWarpingOffsetY,
		ActionData->PlayerWarpingOffsetZ,
		ActionData->PlayerDistanceToEnemy
	};
	WarpingStrategy->ApplyWarp(WarpingData, AbilityOwner, EnemyRef, WarpingLocation, WarpingRotation);
	if (ActionData->MotionWarpingType == EMotionWarpingType::OnGroundAttack)
	{
		WarpingLocationFinal = WarpingLocation;
		WarpingRotationFinal = WarpingRotation;
		AbilityOwner->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocationAndRotation(FName("OnGroundCombo"), WarpingLocationFinal, WarpingRotationFinal);
	}
}

void URHComboGameplayAbility::TryPlayMontage()
{
	if (ActionData->AnimMontage)
	{
		UAbilityTask_PlayMontageAndWait* MontageAndWait = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, ActionData->AnimMontage);
		MontageAndWait->OnCompleted.AddDynamic(this, &URHComboGameplayAbility::K2_EndAbility);
		MontageAndWait->OnInterrupted.AddDynamic(this, &URHComboGameplayAbility::K2_EndAbility);
		MontageAndWait->ReadyForActivation();
	}
}

void URHComboGameplayAbility::OnHitWindowOpen(FGameplayEventData Data)
{
	AActor* Owner = GetAvatarActorFromActorInfo();
	if (!Owner) return;
	if (UWeaponHitBox* Hitbox = Owner->FindComponentByClass<UWeaponHitBox>())
	{
		Hitbox->ArmHitbox(CurrentHitParams);
	}
}

void URHComboGameplayAbility::OnHitWindowClose(FGameplayEventData Data)
{
	AActor* Owner = GetAvatarActorFromActorInfo();
	if (!Owner) return;
	if (UWeaponHitBox* Hitbox = Owner->FindComponentByClass<UWeaponHitBox>())
	{
		Hitbox->DisarmHitbox();
	}
}

void URHComboGameplayAbility::PlayAttackSound(FGameplayEventData Data)
{
	AActor* Owner = GetAvatarActorFromActorInfo();
	if (!Owner) return;
	IRHCharacterActionInterface::Execute_PlayAttackSound(Owner, ActionData->AttackSound, ActionData->AttackAudioStartTime);
}
