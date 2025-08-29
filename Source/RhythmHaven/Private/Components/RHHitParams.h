// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Enum/CameraShakeType.h"
#include "Enum/MotionWarpingType.h"
#include "RHHitParams.generated.h"

USTRUCT(BlueprintType)
struct FRHHitParams
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly)
	TSubclassOf<class UGameplayEffect> EffectClass = nullptr;

	UPROPERTY(BlueprintReadOnly)
	float Damage = 0.f;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<USoundBase> EnemyHitSound = nullptr;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UAnimMontage> EnemyHitMontage = nullptr;

	UPROPERTY(BlueprintReadOnly)
	bool EnemyUseMotionWarping = false;

	UPROPERTY(BlueprintReadOnly)
	EEnemyMotionWarpingType EnemyMotionWarpingType = EEnemyMotionWarpingType::OnGroundHit;

	UPROPERTY(BlueprintReadOnly)
	float EnemyHitBackDistance = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float EnemyWarpingOffsetX = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float EnemyWarpingOffsetY = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float EnemyWarpingOffsetZ = 0.f;

	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EMovementMode> EnemyMovementModeWhenHit = TEnumAsByte<EMovementMode>();

	UPROPERTY(BlueprintReadOnly)
	float EnemyInterpSpeed = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float EnemyInterpTimeLength = 0.f;

	UPROPERTY(BlueprintReadOnly)
	bool bPlayCameraShake;

	UPROPERTY(BlueprintReadOnly)
	ECameraShakeType CameraShakeType;
};
