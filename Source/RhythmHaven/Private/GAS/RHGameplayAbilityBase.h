// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "RHGameplayAbilityBase.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class RHYTHMHAVEN_API URHGameplayAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category="Action Data")
	TObjectPtr<class UGameplayAbilityData> ActionData;

	UPROPERTY()
	TObjectPtr<class ARHCharacterBase> AbilityOwner;
	
private:
	void TryMotionWarping();
	void TryPlayMontage();

	FVector WarpingLocation;
	FRotator WarpingRotation;

	FVector WarpingLocationFinal;
	FRotator WarpingRotationFinal;
};
