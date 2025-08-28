// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EMotionWarpingType : uint8
{
	OnGroundAttack,
	Dodge
};

UENUM(BlueprintType)
enum class EEnemyMotionWarpingType : uint8
{
	OnGroundHit
};
