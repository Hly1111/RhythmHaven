// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/GameplayAbilityData.h"
#include "UObject/Object.h"
#include "MotionWarpingStrategyBase.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class RHYTHMHAVEN_API UMotionWarpingStrategyBase : public UObject
{
	GENERATED_BODY()

public:
	virtual void ApplyWarp(UGameplayAbilityData& AbilityData, AActor* Actor1, AActor* Actor2, FVector& OutWarpLocation, FRotator& OutWarpRotation){}
};
