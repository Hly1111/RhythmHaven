// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MotionWarpingData.generated.h"

USTRUCT(BlueprintType)
struct FMotionWarpingData
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly)
	float WarpTriggerRange = 0;

	UPROPERTY(BlueprintReadOnly)
	float WarpingOffsetX = 0;

	UPROPERTY(BlueprintReadOnly)
	float WarpingOffsetY = 0;

	UPROPERTY(BlueprintReadOnly)
	float WarpingOffsetZ = 0;

	UPROPERTY(BlueprintReadOnly)
	float DistanceToEnemy = 0;
};

USTRUCT(BlueprintType)
struct FEnemyMotionWarpingData
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly)
	float HitBackDistance = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float WarpingOffsetX = 0;

	UPROPERTY(BlueprintReadOnly)
	float WarpingOffsetY = 0;

	UPROPERTY(BlueprintReadOnly)
	float WarpingOffsetZ = 0;

	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EMovementMode> EnemyMovementModeWhenHit = TEnumAsByte<EMovementMode>();

	UPROPERTY(BlueprintReadOnly)
	float InterpSpeed = 0;

	UPROPERTY(BlueprintReadOnly)
	float InterpTimeLength = 0;
};
