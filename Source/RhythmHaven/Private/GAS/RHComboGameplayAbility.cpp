// Fill out your copyright notice in the Description page of Project Settings.


#include "RHComboGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
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
#include "Kismet/GameplayStatics.h"

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
		ActionData->EnemyHitMontage,
		ActionData->EnemyUseMotionWarping,
		ActionData->EnemyMotionWarpingType,
		ActionData->EnemyHitBackDistance,
		ActionData->EnemyWarpingOffsetX,
		ActionData->EnemyWarpingOffsetY,
		ActionData->EnemyWarpingOffsetZ,
		ActionData->EnemyMovementModeWhenHit,
		ActionData->EnemyInterpSpeed,
		ActionData->EnemyInterpTimeLength
	};
	TryHitBoxSetUp();
	
	TryMotionWarping();
	TryPlayMontage();
	WaitForHit();
}

void URHComboGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	bHasHitTriggered = false;
	SendGameplayEvent(UGameplayTagsManager::Get().RequestGameplayTag("Event.DisableHitBox"), FGameplayEventData());
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
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

void URHComboGameplayAbility::WaitForHit()
{
	if (AbilityOwner->ActorHasTag("Player"))
	{
		auto* WaitForHitTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, UGameplayTagsManager::Get().RequestGameplayTag("Attack.Hit"));
		WaitForHitTask->EventReceived.AddDynamic(this, &URHComboGameplayAbility::HandleOnHit);
		WaitForHitTask->ReadyForActivation();
	}
	else if (AbilityOwner->ActorHasTag("Enemy"))
	{
		auto* WaitForHitTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, UGameplayTagsManager::Get().RequestGameplayTag("Attack.HitPlayer"));
		WaitForHitTask->EventReceived.AddDynamic(this, &URHComboGameplayAbility::HandleOnHit);
		WaitForHitTask->ReadyForActivation();
	}
}

void URHComboGameplayAbility::OnHitWindowOpen(FGameplayEventData Data)
{
	if (!AbilityOwner) return;
	if (UWeaponHitBox* Hitbox = AbilityOwner->FindComponentByClass<UWeaponHitBox>())
	{
		Hitbox->ArmHitbox(CurrentHitParams);
	}
}

void URHComboGameplayAbility::OnHitWindowClose(FGameplayEventData Data)
{
	if (!AbilityOwner) return;
	if (UWeaponHitBox* Hitbox = AbilityOwner->FindComponentByClass<UWeaponHitBox>())
	{
		Hitbox->DisarmHitbox();
	}
}

void URHComboGameplayAbility::PlayAttackSound(FGameplayEventData Data)
{
	if (!AbilityOwner) return;
	IRHCharacterActionInterface::Execute_PlayAttackSound(AbilityOwner, ActionData->AttackSound, ActionData->AttackAudioStartTime);
}

void URHComboGameplayAbility::HandleOnHit(FGameplayEventData Data)
{
	AActor* EnemyActor = const_cast<AActor*>(Data.Target.Get());
	UAbilitySystemComponent* AbilitySystemComponent = AbilityOwner->GetAbilitySystemComponent();
	//伤害
	FGameplayEffectContextHandle Ctx = AbilitySystemComponent->MakeEffectContext();
	Ctx.AddInstigator(AbilityOwner,AbilityOwner);
	FGameplayEffectSpecHandle Spec = AbilitySystemComponent->MakeOutgoingSpec(ActionData->GameplayEffectToApply, 1.f, Ctx);
	if (Spec.IsValid())
	{
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(Spec, UGameplayTagsManager::Get().RequestGameplayTag("Data.Damage"), ActionData->Damage);
		AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*Spec.Data.Get(), UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(EnemyActor));
	}
	
	//音效
	if (ActionData->EnemyHitSound)
	{
		FGameplayCueParameters GCParams;
		GCParams.SourceObject  = ActionData->EnemyHitSound;
		AbilityOwner->GetAbilitySystemComponent()->ExecuteGameplayCue(UGameplayTagsManager::Get().RequestGameplayTag(FName("GameplayCue.Hit.Sound")), GCParams);
	}

	if (bHasHitTriggered) return;
	//摄像机震动
	if (ActionData->bPlayCameraShake)
	{
		FGameplayCueParameters GCParams;
		GCParams.Instigator = AbilityOwner;
		if (ActionData->CameraShakeType == ECameraShakeType::Light)
		{
			AbilityOwner->GetAbilitySystemComponent()->ExecuteGameplayCue(UGameplayTagsManager::Get().RequestGameplayTag(FName("GameplayCue.Hit.CameraShake.Light")), GCParams);
		}
		else if (ActionData->CameraShakeType == ECameraShakeType::Heavy)
		{
			AbilityOwner->GetAbilitySystemComponent()->ExecuteGameplayCue(UGameplayTagsManager::Get().RequestGameplayTag(FName("GameplayCue.Hit.CameraShake.Heavy")), GCParams);
		}
	}
	//顿帧

	if (ActionData->bUseTimeDilation)
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.2f);
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
		{
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
		}, ActionData->TimeDilationDuration, false);
	}
	bHasHitTriggered = true;
}
