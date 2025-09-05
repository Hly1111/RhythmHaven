// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RHBaseGameAbility.h"
#include "Enum/LocomotionDirection.h"
#include "RHDodgeAbility.generated.h"

/**
 * 
 */

UCLASS()
class RHYTHMHAVEN_API URHDodgeAbility : public URHBaseGameAbility
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly)
	TMap<ELocomotionDirection, TObjectPtr<UAnimMontage>> DodgeMontages;

	UPROPERTY()
	TObjectPtr<ACharacter> Owner;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
};
