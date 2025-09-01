// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Framework/RhythmTypes.h"
#include "RhythmComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBeatHit,int32,Bar,int32,Beat);
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RHYTHMHAVEN_API URhythmComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable, Category="Rhythm")
	FOnBeatHit OnBeatEvent;

	UFUNCTION(BlueprintCallable, Category="Rhythm")
	void StartOnBeatEvent();

	UPROPERTY(EditDefaultsOnly, Category="Rhythm")
	bool bStartFromBeginning = true;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnBeatTriggered(int32 Bar,int32 Beat) const;

	UPROPERTY(EditDefaultsOnly, Category="Rhythm System", meta=(AllowPrivateAccess=true))
	FSyncEventData SyncEventData;
};
