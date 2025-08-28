// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RHBaseGameAbility.h"
#include "Components/MotionWarping/MotionWarpingData.h"
#include "RHGetHitAbility.generated.h"

/**
 * 
 */
UCLASS()
class RHYTHMHAVEN_API URHGetHitAbility : public URHBaseGameAbility
{
	GENERATED_BODY()
	
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
private:
	UPROPERTY()
	TObjectPtr<class ARHCharacterBase> AbilityOwner;

	UPROPERTY()
	TObjectPtr<AActor> AttackInstigator;
};
