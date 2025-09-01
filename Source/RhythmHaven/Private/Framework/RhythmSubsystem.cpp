// Fill out your copyright notice in the Description page of Project Settings.


#include "RhythmSubsystem.h"
#include "Engine/Engine.h"

void URhythmSubsystem::InitializeSystem(float InBpm, int32 BeatPerBar, EQuartzTimeSignatureQuantization BeatType,
                                  bool bStartAtNextBar)
{
	Quartz =  GetWorld() ? GetWorld()->GetSubsystem<UQuartzSubsystem>() : nullptr;
	if (!Quartz) return;

	CurrentBpm = InBpm;
	ClockSettings.TimeSignature.NumBeats = BeatPerBar;
	ClockSettings.TimeSignature.BeatType = BeatType;

	Quartz->CreateNewClock(GetWorld(), ClockName, ClockSettings, true);
	ClockHandle = Quartz->GetHandleForClock(GetWorld(), ClockName);
	if (ClockHandle)
	{
		FQuartzQuantizationBoundary Boundary;
		Boundary.Quantization = EQuartzCommandQuantization::None;
		FOnQuartzCommandEventBP DummyDelegate;
		UQuartzClockHandle* Handle = ClockHandle.Get();
		ClockHandle->SetBeatsPerMinute(this, Boundary, DummyDelegate, Handle, CurrentBpm);
	}
	if (bStartAtNextBar)
	{
		StartClock(true);
	}
}

void URhythmSubsystem::SetBPM(float InBpm, EQuartzCommandQuantization When)
{
	if (!ClockHandle) return;
	CurrentBpm = InBpm;

	FQuartzQuantizationBoundary Boundary;
	Boundary.Quantization = When;
	FOnQuartzCommandEventBP DummyDelegate;
	UQuartzClockHandle* Handle = ClockHandle.Get();
	ClockHandle->SetBeatsPerMinute(this, Boundary, DummyDelegate, Handle, CurrentBpm);
}

void URhythmSubsystem::StartClock(bool bStartNextBar)
{
	if (!ClockHandle) return;
	FQuartzQuantizationBoundary Boundary;
	Boundary.Quantization = bStartNextBar? EQuartzCommandQuantization::Bar: EQuartzCommandQuantization::None;

	UQuartzClockHandle* Handle = ClockHandle.Get();
	ClockHandle->StartClock(this, Handle);
	bStarted = true;
	InternalSubscribe(EQuartzCommandQuantization::Beat);
}

void URhythmSubsystem::StopClock()
{
	if (!ClockHandle) return;
	UQuartzClockHandle* Handle = ClockHandle.Get();
	ClockHandle->StopClock(this, true, Handle);
	bStarted = false;
}

void URhythmSubsystem::Deinitialize()
{
	Quartz = nullptr;
	ClockHandle = nullptr;
	bStarted = false;
}

void URhythmSubsystem::InternalSubscribe(EQuartzCommandQuantization Quantization)
{
	if (!ClockHandle) return;
	FOnQuartzMetronomeEventBP Dyn;
	Dyn.BindUFunction(this, TEXT("OnMetronome"));
	UQuartzClockHandle* Handle = ClockHandle.Get();
	ClockHandle->SubscribeToQuantizationEvent(this, Quantization, Dyn, Handle);
}

void URhythmSubsystem::OnMetronome(FName InClockName, EQuartzCommandQuantization InQuantization, int32 InBar,
	int32 InBeat)
{
	const FString TagName = FString::Printf(TEXT("RSS.NoteCount.%02d"), InBeat);
	const FGameplayTag CurrentTag = FGameplayTag::RequestGameplayTag(FName(*TagName));
	for (int32 i = OnRhythmEvents.Num() - 1; i >= 0; --i)
	{
		FSyncEventData& Event = OnRhythmEvents[i];
		
		if (!Event.bActivated)
		{
			if (Event.StartTags.Contains(CurrentTag))
			{
				Event.bActivated = true;
			}
			else
			{
				continue;
			}
		}
		if (Event.CallTags.Contains(CurrentTag))
		{
			if (Event.Callback.ExecuteIfBound(InBar, InBeat))
			{
				if (Event.LoopCount > 0)
				{
					++Event.TriggeredCount;
					if (Event.TriggeredCount >= Event.LoopCount)
					{
						OnRhythmEvents.RemoveAt(i);
					}
				}
			}
		}
	}
}

void URhythmSubsystem::SetSyncEvent(const FSyncEventData& EventCallback)
{
	OnRhythmEvents.Add(EventCallback);
}











