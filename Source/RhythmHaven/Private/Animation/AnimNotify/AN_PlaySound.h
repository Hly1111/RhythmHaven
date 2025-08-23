// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_PlaySound.generated.h"

/**
 * 
 */
UCLASS()
class RHYTHMHAVEN_API UAN_PlaySound : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY(EditAnywhere, Category = "Anim Notify Settings")
	TObjectPtr<class USoundBase> SoundToPlay;
};
