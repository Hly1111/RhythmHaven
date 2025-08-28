// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/RHHitParams.h"
#include "Enum/MotionWarpingType.h"
#include "UObject/Object.h"
#include "RHHitPayload.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class RHYTHMHAVEN_API URHHitPayload : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly)
	TSubclassOf<class UGameplayEffect> EffectClass = nullptr;
	
	UPROPERTY(BlueprintReadOnly)
	float Damage = 0.f;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class USoundBase> HitSound = nullptr;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class UAnimMontage> HitMontage = nullptr;

	UPROPERTY(BlueprintReadOnly)
	bool bUseMotionWarping;

	UPROPERTY(BlueprintReadOnly)
	EEnemyMotionWarpingType EnemyMotionWarpingType;

	UPROPERTY(BlueprintReadOnly)
	float EnemyHitBackDistance;

	UPROPERTY(BlueprintReadOnly)
	float EnemyWarpingOffsetX;

	UPROPERTY(BlueprintReadOnly)
	float EnemyWarpingOffsetY;

	UPROPERTY(BlueprintReadOnly)
	float EnemyWarpingOffsetZ;

	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EMovementMode> EnemyMovementModeWhenHit;

	UPROPERTY(BlueprintReadOnly)
	float EnemyInterpSpeed;

	UPROPERTY(BlueprintReadOnly)
	float EnemyInterpTimeLength;

	UFUNCTION()
	void PassInData(const FRHHitParams& HitData);
};
