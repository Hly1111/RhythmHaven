// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EMotionWarpingType : uint8
{
	OnGroundAttack,
	InAirAttack,
	Dodge
};

UENUM(BlueprintType)
enum class EEnemyMotionWarpingType : uint8
{
	None
};
