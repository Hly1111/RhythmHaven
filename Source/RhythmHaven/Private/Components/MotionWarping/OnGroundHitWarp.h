// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MotionWarpingStrategyBase.h"
#include "OnGroundHitWarp.generated.h"

/**
 * 
 */
UCLASS()
class RHYTHMHAVEN_API UOnGroundHitWarp : public UMotionWarpingStrategyBase
{
	GENERATED_BODY()
	
protected:
	virtual void ApplyWarp(const FEnemyMotionWarpingData& Data, AActor* Actor1, AActor* Actor2, FVector& OutWarpLocation, FRotator& OutWarpRotation) override;
};
