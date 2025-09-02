// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/RHBaseGameAbility.h"
#include "Components/RHHitParams.h"
#include "RHComboGameplayAbility.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class RHYTHMHAVEN_API URHComboGameplayAbility : public URHBaseGameAbility
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

	FRHHitParams CurrentHitParams;
	
private:
	void TryHitBoxSetUp();
	void TryMotionWarping();
	void TryPlayMontage();
	void WaitForHit();

	bool bHasHitTriggered = false;

	FVector WarpingLocation;
	FRotator WarpingRotation;

	FVector WarpingLocationFinal;
	FRotator WarpingRotationFinal;

	UFUNCTION()
	void OnHitWindowOpen(FGameplayEventData Data);

	UFUNCTION()
	void OnHitWindowClose(FGameplayEventData Data);

	UFUNCTION()
	void PlayAttackSound(FGameplayEventData Data);

	UFUNCTION()
	void HandleOnHit(FGameplayEventData Payload);
};
