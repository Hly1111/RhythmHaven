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

	UPROPERTY(EditDefaultsOnly, Category = "Motion Warping Strategy")
	TMap<EEnemyMotionWarpingType, TSubclassOf<class UMotionWarpingStrategyBase>> EnemyMotionWarpingStrategies;

	UPROPERTY()
	TMap<EEnemyMotionWarpingType, TObjectPtr<UMotionWarpingStrategyBase>> EnemyMotionWarpingStrategyPool;

public:
	UMotionWarpingStrategyBase* GetStrategy(EMotionWarpingType MotionWarpingType);
	UMotionWarpingStrategyBase* GetStrategy(EEnemyMotionWarpingType MotionWarpingType);

	void StartWarpingTimer(AActor* Target, float WarpInterpSpeed, float WarpInterpTime, FVector WarpLocation, FRotator WarpRotation);

private:
	FTimerHandle WarpingTimerHandle;
	float WarpingElapsed = 0.f;

	UFUNCTION()
	void DoWarp(AActor* Target, float WarpInterpSpeed, float WarpInterpTime, const FVector& WarpLocation, const FRotator& WarpRotation);
};
