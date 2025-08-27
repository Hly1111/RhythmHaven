// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Enum/CameraShakeType.h"
#include "Enum/MotionWarpingType.h"
#include "GameplayAbilityData.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class RHYTHMHAVEN_API UGameplayAbilityData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Action Data")
	TObjectPtr<UAnimMontage> AnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Action Data")
	bool bUseMotionWarping;

	UPROPERTY(EditDefaultsOnly, Category = "Action Data")
	EMotionWarpingType MotionWarpingType;

	UPROPERTY(EditDefaultsOnly, Category = "Action Data")
	float PlayerWarpTriggerRange;

	UPROPERTY(EditDefaultsOnly, Category = "Action Data")
	float PlayerDistanceToEnemy;

	UPROPERTY(EditDefaultsOnly, Category = "Action Data")
	float PlayerWarpingOffsetX;

	UPROPERTY(EditDefaultsOnly, Category = "Action Data")
	float PlayerWarpingOffsetY;

	UPROPERTY(EditDefaultsOnly, Category = "Action Data")
	float PlayerWarpingOffsetZ;

	UPROPERTY(EditDefaultsOnly, Category = "Action Data")
	float PlayerInterpSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Action Data")
	float PlayerInterpTimeLength;

	UPROPERTY(EditDefaultsOnly, Category = "Action Data")
	TObjectPtr<UAnimMontage> EnemyHitMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Action Data")
	float AttackAudioStartTime;

	UPROPERTY(EditDefaultsOnly, Category = "Action Data")
	TObjectPtr<USoundBase> AttackSound;

	UPROPERTY(EditDefaultsOnly, Category = "Action Data")
	TObjectPtr<USoundBase> EnemyHitSound;

	UPROPERTY(EditDefaultsOnly, Category = "Action Data")
	bool EnemyUseMotionWarping;

	UPROPERTY(EditDefaultsOnly, Category = "Action Data")
	EEnemyMotionWarpingType EnemyMotionWarpingType;

	UPROPERTY(EditDefaultsOnly, Category = "Action Data")
	float EnemyHitBackDistance;

	UPROPERTY(EditDefaultsOnly, Category = "Action Data")
	float EnemyWarpingOffsetX;

	UPROPERTY(EditDefaultsOnly, Category = "Action Data")
	float EnemyWarpingOffsetY;

	UPROPERTY(EditDefaultsOnly, Category = "Action Data")
	float EnemyWarpingOffsetZ;

	UPROPERTY(EditDefaultsOnly, Category = "Action Data")
	TEnumAsByte<EMovementMode> EnemyMovementModeWhenHit;

	UPROPERTY(EditDefaultsOnly, Category = "Action Data")
	float EnemyInterpSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Action Data")
	float EnemyInterpTimeLength;

	UPROPERTY(EditDefaultsOnly, Category = "Action Data")
	bool bPlayCameraShake;

	UPROPERTY(EditDefaultsOnly, Category = "Action Data")
	ECameraShakeType CameraShakeType;

	UPROPERTY(EditDefaultsOnly, Category = "Action Data")
	TSubclassOf<class UGameplayEffect> GameplayEffectToApply;
};
