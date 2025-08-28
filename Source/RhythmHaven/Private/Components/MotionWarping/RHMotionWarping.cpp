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

	for (auto Pair : EnemyMotionWarpingStrategies)
	{
		const EEnemyMotionWarpingType Type = Pair.Key;
		if (EnemyMotionWarpingStrategyPool.Contains(Type))
		{
			continue;
		}
		const TSubclassOf<UMotionWarpingStrategyBase> StrategyClass = Pair.Value;
		if (!StrategyClass)
		{
			continue;
		}
		UMotionWarpingStrategyBase* Instance = NewObject<UMotionWarpingStrategyBase>(this, StrategyClass);
		EnemyMotionWarpingStrategyPool.Add(Type, Instance);
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

UMotionWarpingStrategyBase* URHMotionWarping::GetStrategy(EEnemyMotionWarpingType MotionWarpingType)
{
	if (TObjectPtr<UMotionWarpingStrategyBase>* Found = EnemyMotionWarpingStrategyPool.Find(MotionWarpingType))
	{
		return Found->Get();
	}
	return nullptr;
}

void URHMotionWarping::StartWarpingTimer(AActor* Target, float WarpInterpSpeed, float WarpInterpTime, FVector WarpLocation, FRotator WarpRotation)
{
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, FName("DoWarp"), Target, WarpInterpSpeed, WarpInterpTime, WarpLocation,WarpRotation);
	GetWorld()->GetTimerManager().SetTimer(WarpingTimerHandle, TimerDelegate, 0.01f,true);
}

void URHMotionWarping::DoWarp(AActor* Target, float WarpInterpSpeed, float WarpInterpTime, const FVector& WarpLocation, const FRotator& WarpRotation)
{
	FVector Location = FMath::VInterpTo(Target->GetActorLocation(), WarpLocation, GetWorld()->GetDeltaSeconds(),WarpInterpSpeed);
	Target->SetActorLocation(Location);
	float WarpingTimer = 0;
	float Yaw = FMath::RInterpTo(Target->GetActorRotation(), WarpRotation, GetWorld()->GetDeltaSeconds(), 10.f).Yaw;
	FRotator Rotation = FRotator(Target->GetActorRotation().Pitch, Yaw, Target->GetActorRotation().Roll);
	Target->SetActorRotation(Rotation);
	WarpingTimer += GetWorld()->GetDeltaSeconds();
	if (WarpingTimer >= WarpInterpTime)
	{
		WarpingTimerHandle.Invalidate();
	}
}



