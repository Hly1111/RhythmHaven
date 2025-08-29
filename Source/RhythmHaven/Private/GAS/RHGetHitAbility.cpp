// Fill out your copyright notice in the Description page of Project Settings.


#include "RHGetHitAbility.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayTagsManager.h"
#include "PropertyAccess.h"
#include "RHHitPayload.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Character/RHCharacterBase.h"
#include "Components/MotionWarping/MotionWarpingStrategyBase.h"
#include "Components/MotionWarping/RHMotionWarping.h"
#include "Kismet/GameplayStatics.h"

void URHGetHitAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                       const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                       const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!K2_CommitAbility())
	{
		K2_EndAbility();
		return;
	}
	
	AActor* Target = ActorInfo && ActorInfo->AvatarActor.IsValid() ? ActorInfo->AvatarActor.Get() : nullptr;
	AbilityOwner = Cast<ARHCharacterBase>(Target);
	UAbilitySystemComponent* TargetASC = ActorInfo ? ActorInfo->AbilitySystemComponent.Get() : nullptr;
	
	if (!AbilityOwner || !TargetASC)
	{
		K2_EndAbility();
		return;
	}

	if (const URHHitPayload* Payload = TriggerEventData ? Cast<URHHitPayload>(TriggerEventData->OptionalObject) : nullptr)
	{
		AttackInstigator = const_cast<AActor*>(TriggerEventData->Instigator.Get());
		if (UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(AttackInstigator))
		{
			//伤害
			FGameplayEffectContextHandle Ctx = SourceASC->MakeEffectContext();
			Ctx.AddInstigator(AttackInstigator,AttackInstigator);
			FGameplayEffectSpecHandle Spec = SourceASC->MakeOutgoingSpec(Payload->EffectClass, 1.f, Ctx);
			if (Spec.IsValid())
			{
				UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(Spec, UGameplayTagsManager::Get().RequestGameplayTag("Data.Damage"), Payload->Damage);
				SourceASC->ApplyGameplayEffectSpecToTarget(*Spec.Data.Get(), TargetASC);
			}

			//音效
			if (Payload->HitSound)
			{
				FGameplayCueParameters GCParams;
				GCParams.EffectContext = Ctx;
				GCParams.SourceObject  = Payload->HitSound;
				SourceASC->ExecuteGameplayCue(UGameplayTagsManager::Get().RequestGameplayTag(FName("GameplayCue.Hit.Sound")), GCParams);
			}

			//摄像机震动
			if (Payload->bPlayCameraShake)
			{
				FGameplayCueParameters GCParams;
				GCParams.EffectContext = Ctx;
				GCParams.Instigator = AttackInstigator;
				if (Payload->CameraShakeType == ECameraShakeType::Light)
				{
					SourceASC->ExecuteGameplayCue(UGameplayTagsManager::Get().RequestGameplayTag(FName("GameplayCue.Hit.CameraShake.Light")), GCParams);
				}
				else if (Payload->CameraShakeType == ECameraShakeType::Heavy)
				{
					SourceASC->ExecuteGameplayCue(UGameplayTagsManager::Get().RequestGameplayTag(FName("GameplayCue.Hit.CameraShake.Heavy")), GCParams);
				}
			}
			//顿帧

			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.2f);
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
			{
				UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
			}, 0.01f, false); 
			
			//MotionWarping
			if (Payload->bUseMotionWarping)
			{
				UMotionWarpingStrategyBase* WarpingStrategy = AbilityOwner->GetMotionWarpingComponent()->GetStrategy(Payload->EnemyMotionWarpingType);
				FEnemyMotionWarpingData Data = FEnemyMotionWarpingData
				{
					Payload->EnemyHitBackDistance,
					Payload->EnemyWarpingOffsetX,
					Payload->EnemyWarpingOffsetY,
					Payload->EnemyWarpingOffsetZ,
					Payload->EnemyMovementModeWhenHit,
					Payload->EnemyInterpSpeed,
					Payload->EnemyInterpTimeLength
				};
				FVector OutWarpLocation;
				FRotator OutWarpRotation;
				WarpingStrategy->ApplyWarp(Data, AbilityOwner/* 敌人 */, AttackInstigator/* 玩家 */, OutWarpLocation, OutWarpRotation);
				AbilityOwner->GetMotionWarpingComponent()->StartWarpingTimer(AbilityOwner, Data.InterpSpeed, Data.InterpTimeLength, OutWarpLocation, OutWarpRotation);
			}
			
			//Montage
			UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, Payload->HitMontage);
			PlayMontageTask->OnCompleted.AddDynamic(this, &URHGetHitAbility::K2_EndAbility);
			PlayMontageTask->OnInterrupted.AddDynamic(this, &URHGetHitAbility::K2_EndAbility);
			PlayMontageTask->OnBlendedIn.AddDynamic(this, &URHGetHitAbility::K2_EndAbility);
			PlayMontageTask->OnBlendOut.AddDynamic(this, &URHGetHitAbility::K2_EndAbility);
			PlayMontageTask->ReadyForActivation();
		}
	}
	
}

void URHGetHitAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
