// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MotionWarpingStrategyBase.h"
#include "OnGroundComboWarp.generated.h"

/**
 * 
 */
UCLASS()
class RHYTHMHAVEN_API UOnGroundComboWarp : public UMotionWarpingStrategyBase
{
	GENERATED_BODY()

public:
	virtual void ApplyWarp(const FMotionWarpingData& Data, AActor* Actor1, AActor* Actor2, FVector& OutWarpLocation, FRotator& OutWarpRotation) override;
};
