// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Enum/MovementType.h"
#include "AN_ChangeMovementType.generated.h"

/**
 * 
 */
UCLASS()
class RHYTHMHAVEN_API UAN_ChangeMovementType : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	UPROPERTY(EditAnywhere, Category="Anim Notify Settings")
	EMovementType MovementType;

	UPROPERTY(EditAnywhere, Category="Anim Notify Settings")
	float MaxWalkSpeed;
};
