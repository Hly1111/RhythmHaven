// Fill out your copyright notice in the Description page of Project Settings.


#include "RhythmComponent.h"
#include "Framework/RhythmSubsystem.h"


void URhythmComponent::StartOnBeatEvent()
{
	SyncEventData.TriggeredCount = 0;
	SyncEventData.bActivated = (SyncEventData.StartTags.Num() == 0);
	SyncEventData.Callback.Unbind();
	SyncEventData.Callback.BindUFunction(this, FName("OnBeatTriggered"));

	if (UWorld* World = GetWorld())
	{
		if (UGameInstance* GI = World->GetGameInstance())
		{
			if (URhythmSubsystem* RhythmSystem = GI->GetSubsystem<URhythmSubsystem>())
			{
				RhythmSystem->SetSyncEvent(SyncEventData);
			}
		}
	}
	
}

void URhythmComponent::BeginPlay()
{
	Super::BeginPlay();
	if (!bStartFromBeginning) return;
	SyncEventData.bActivated = (SyncEventData.StartTags.Num() == 0);
	SyncEventData.Callback.BindUFunction(this, FName("OnBeatTriggered"));
	
	if (GetWorld() && GetWorld()->GetGameInstance())
	{
		if (URhythmSubsystem* RhythmSystem = GetWorld()->GetGameInstance()->GetSubsystem<URhythmSubsystem>())
		{
			RhythmSystem->SetSyncEvent(SyncEventData);
		}
	}
}

void URhythmComponent::OnBeatTriggered(int32 Bar, int32 Beat) const
{
	if (OnBeatEvent.IsBound())
	{
		OnBeatEvent.Broadcast(Bar, Beat);
	}
}
