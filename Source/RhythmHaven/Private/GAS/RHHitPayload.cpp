// Fill out your copyright notice in the Description page of Project Settings.


#include "RHHitPayload.h"

void URHHitPayload::PassInData(const FRHHitParams& HitData)
{
	EffectClass = HitData.EffectClass;
	Damage = HitData.Damage;
	HitMontage = HitData.EnemyHitMontage;
	HitSound = HitData.EnemyHitSound;
	bUseMotionWarping = HitData.EnemyUseMotionWarping;
	EnemyMotionWarpingType = HitData.EnemyMotionWarpingType;
	EnemyHitBackDistance = HitData.EnemyHitBackDistance;
	EnemyWarpingOffsetX = HitData.EnemyWarpingOffsetX;
	EnemyWarpingOffsetY = HitData.EnemyWarpingOffsetY;
	EnemyWarpingOffsetZ = HitData.EnemyWarpingOffsetZ;
	EnemyMovementModeWhenHit = HitData.EnemyMovementModeWhenHit;
	EnemyInterpSpeed = HitData.EnemyInterpSpeed;
	EnemyInterpTimeLength = HitData.EnemyInterpTimeLength;
}
