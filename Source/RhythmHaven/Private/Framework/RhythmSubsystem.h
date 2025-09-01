// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RhythmTypes.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Quartz/QuartzSubsystem.h"
#include "Quartz/AudioMixerClockHandle.h"
#include "RhythmSubsystem.generated.h"

UCLASS()
class RHYTHMHAVEN_API URhythmSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()
public:
    UPROPERTY()
    TArray<FSyncEventData> OnRhythmEvents;

    // ===== Quartz =====
    UFUNCTION(BlueprintCallable, Category="Rhythm")
    void InitializeSystem(float InBpm = 120.f,
                          int32 BeatPerBar = 4,
                          EQuartzTimeSignatureQuantization BeatType = EQuartzTimeSignatureQuantization::QuarterNote,
                          bool bStartAtNextBar = true);

    UFUNCTION(BlueprintCallable, Category="Rhythm")
    void SetBPM(float InBpm, EQuartzCommandQuantization When = EQuartzCommandQuantization::None);

    UFUNCTION(BlueprintCallable, Category="Rhythm")
    void StartClock(bool bStartNextBar = true);

    UFUNCTION(BlueprintCallable, Category="Rhythm")
    void StopClock();

    UFUNCTION(BlueprintPure, Category="Rhythm")
    float GetCurrentBPM() const { return CurrentBpm; }

protected:
    virtual void Deinitialize() override;

private:
    void InternalSubscribe(EQuartzCommandQuantization Quantization);

    UFUNCTION()
    void OnMetronome(FName InClockName, EQuartzCommandQuantization InQuantization, int32 InBar, int32 InBeat);
private:
    UPROPERTY()
	TObjectPtr<UQuartzSubsystem> Quartz;
    UPROPERTY()
	TObjectPtr<UQuartzClockHandle> ClockHandle;

    UPROPERTY() FName ClockName = TEXT("RhythmClock");

    FQuartzClockSettings ClockSettings;
    float CurrentBpm = 120.f;
    bool  bStarted = false;
    
    /* Component Setting*/
public:
    void SetSyncEvent(const FSyncEventData& EventCallback);
};