// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/RHCharacterActionInterface.h"
#include "RHCharacterBase.generated.h"

UCLASS(Abstract)
class RHYTHMHAVEN_API ARHCharacterBase : public ACharacter, public IRHCharacterActionInterface
{
	GENERATED_BODY()

public:
	ARHCharacterBase();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Character Components")
	TObjectPtr<class URHHit> HitComponent;

	UPROPERTY(VisibleAnywhere, Category = "Character Components")
	TObjectPtr<class URHAction> ActionComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	TSubclassOf<UAnimInstance> LinkAnimLayerClass;
};
