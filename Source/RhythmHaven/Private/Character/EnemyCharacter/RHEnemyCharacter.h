// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/RHCharacterBase.h"
#include "Components/RHHitParams.h"
#include "RHEnemyCharacter.generated.h"

UCLASS(Abstract)
class RHYTHMHAVEN_API ARHEnemyCharacter : public ARHCharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARHEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/* Interface */

	virtual void EnemyReceiveDamage_Implementation(FRHHitParams HitData, ACharacter* AttackInstigator) override;
	virtual bool SetMovementMode_Implementation(EMovementMode Mode) override;
	virtual bool IsMovingOnGround_Implementation() override;
};
