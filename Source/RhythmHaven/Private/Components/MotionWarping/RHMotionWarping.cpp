// Fill out your copyright notice in the Description page of Project Settings.


#include "RHMotionWarping.h"
#include "MotionWarpingStrategyBase.h"

void URHMotionWarping::InitialMotionWarpingStrategyPool()
{
	MotionWarpingStrategyPool.Reserve(10);
	for (auto Pair : MotionWarpingStrategies)
	{
		const EMotionWarpingType Type = Pair.Key;
		if (MotionWarpingStrategyPool.Contains(Type))
		{
			continue;
		}
		const TSubclassOf<UMotionWarpingStrategyBase> StrategyClass = Pair.Value;
		if (!StrategyClass)
		{
			continue;
		}
		UMotionWarpingStrategyBase* Instance = NewObject<UMotionWarpingStrategyBase>(this, StrategyClass);
		MotionWarpingStrategyPool.Add(Type, Instance);
	}
}

UMotionWarpingStrategyBase* URHMotionWarping::GetStrategy(EMotionWarpingType MotionWarpingType)
{
	if (TObjectPtr<UMotionWarpingStrategyBase>* Found = MotionWarpingStrategyPool.Find(MotionWarpingType))
	{
		return Found->Get();
	}
	return nullptr;
}

