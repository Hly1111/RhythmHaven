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
