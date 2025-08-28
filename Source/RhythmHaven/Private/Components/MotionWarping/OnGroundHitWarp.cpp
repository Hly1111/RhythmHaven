// Fill out your copyright notice in the Description page of Project Settings.


#include "OnGroundHitWarp.h"

#include "Interface/RHCharacterActionInterface.h"
#include "Interface/RHCharacterDataInterface.h"
#include "Kismet/KismetMathLibrary.h"

void UOnGroundHitWarp::ApplyWarp(const FEnemyMotionWarpingData& Data, AActor* Actor1/* 敌人 */, AActor* Actor2/* 玩家 */,
                                 FVector& OutWarpLocation, FRotator& OutWarpRotation)
{
	IRHCharacterActionInterface::Execute_SetMovementMode(Actor1, Data.EnemyMovementModeWhenHit);

	FVector Direction = (Actor2->GetActorLocation() - Actor1->GetActorLocation()).GetSafeNormal();
	FVector Offset =
			Data.WarpingOffsetX * Actor2->GetActorForwardVector() +
			Data.WarpingOffsetY * Actor2->GetActorRightVector() +
			Data.WarpingOffsetZ * Actor2->GetActorUpVector();
	if (IRHCharacterDataInterface::Execute_IsMovingOnGround(Actor1))
	{
		OutWarpLocation = Actor1->GetActorLocation() - Data.HitBackDistance * Direction + Offset;
		OutWarpRotation = UKismetMathLibrary::FindLookAtRotation(Actor1->GetActorLocation(), Actor2->GetActorLocation());
#ifdef WITH_EDITOR
		DrawDebugSphere(Actor1->GetWorld(), OutWarpLocation, 30, 12, FColor::Blue, false, 2.f, 0, 1.f);
#endif
	}
	else
	{
		OutWarpLocation = Actor1->GetActorLocation() + Data.HitBackDistance * Direction + Offset;
		OutWarpLocation.Z = Actor2->GetActorLocation().Z;
	}
}
