// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTags.h"
#include "RhythmTypes.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FRhythmSyncEvent, int, BarCount, int, BeatCount);

USTRUCT(BlueprintType)
struct FSyncEventData
{
	GENERATED_BODY()

	UPROPERTY()
	FRhythmSyncEvent Callback;
	
	UPROPERTY(EditDefaultsOnly, Category="Rhythm System")
	TArray<FGameplayTag> StartTags;

	UPROPERTY(EditDefaultsOnly, Category="Rhythm System")
	TArray<FGameplayTag> CallTags;

	UPROPERTY(EditDefaultsOnly, Category="Rhythm System")
	int32 LoopCount = 0;

	UPROPERTY()
	int32 TriggeredCount = 0;

	UPROPERTY()
	bool bActivated = false;
};