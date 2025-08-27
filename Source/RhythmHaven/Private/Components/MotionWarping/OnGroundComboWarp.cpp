// Fill out your copyright notice in the Description page of Project Settings.


#include "OnGroundComboWarp.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"

void UOnGroundComboWarp::ApplyWarp(UGameplayAbilityData& AbilityData, AActor* Actor1, AActor* Actor2, FVector& OutWarpLocation, FRotator& OutWarpRotation)
{
	if (IsValid(Actor2) && AbilityData.PlayerWarpTriggerRange > Actor1->GetDistanceTo(Actor2))
	{
		FVector Direction = (Actor1->GetActorLocation() - Actor2->GetActorLocation()).GetSafeNormal();
		FVector Offset =
			AbilityData.PlayerWarpingOffsetX * Actor1->GetActorForwardVector() +
			AbilityData.PlayerWarpingOffsetY * Actor1->GetActorRightVector() +
			AbilityData.PlayerWarpingOffsetZ * Actor1->GetActorUpVector();

		OutWarpLocation = Actor2->GetActorLocation() + Direction * AbilityData.PlayerDistanceToEnemy + Offset;
		OutWarpRotation = UKismetMathLibrary::FindLookAtRotation(Actor1->GetActorLocation(), Actor2->GetActorLocation());

#ifdef WITH_EDITOR
		DrawDebugSphere(Actor1->GetWorld(), OutWarpLocation, 10, 12, FColor::Green, false, 2.f, 0, 1.f);
#endif
	}
	else
	{
		OutWarpLocation = Actor1->GetActorLocation() + 100.f * Actor1->GetActorForwardVector();
		OutWarpRotation = Actor1->GetActorRotation();
	}
}
