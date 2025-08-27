// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MotionWarpingComponent.h"
#include "Enum/MotionWarpingType.h"
#include "RHMotionWarping.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RHYTHMHAVEN_API URHMotionWarping : public UMotionWarpingComponent
{
	GENERATED_BODY()

public:
	void InitialMotionWarpingStrategyPool();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Motion Warping Strategy")
	TMap<EMotionWarpingType, TSubclassOf<class UMotionWarpingStrategyBase>> MotionWarpingStrategies;

	UPROPERTY()
	TMap<EMotionWarpingType, TObjectPtr<UMotionWarpingStrategyBase>> MotionWarpingStrategyPool;

public:
	UMotionWarpingStrategyBase* GetStrategy(EMotionWarpingType MotionWarpingType);
};
