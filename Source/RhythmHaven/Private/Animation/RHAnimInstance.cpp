// Fill out your copyright notice in the Description page of Project Settings.


#include "RHAnimInstance.h"
#include "KismetAnimationLibrary.h"
#include "GameFramework/Character.h"

void URHAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
	if (OwnerCharacter)
	{
		OwnerMovementComponent = OwnerCharacter->GetCharacterMovement();
	}
}

void URHAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if (OwnerCharacter)
	{
		ActorRotation = OwnerCharacter->GetActorRotation();
		CachedActorRotationYaw = ActorRotationYaw;
		ActorRotationYaw = ActorRotation.Yaw;
		
		ActorLeanAngle = FMath::Clamp((ActorRotationYaw - CachedActorRotationYaw) / DeltaSeconds / 4.f, -60.f, 60.f);
		ActorVelocity2D = OwnerMovementComponent->Velocity * FVector(1.f, 1.f, 0.f);
		const FVector ActorAcceleration2D = OwnerMovementComponent->GetCurrentAcceleration() * FVector(1.f, 1.f, 0.f);
		bIsAccelerating = !FMath::IsNearlyEqual(ActorAcceleration2D.Length(), 0.f);

		MovementMode = OwnerMovementComponent->MovementMode;
		bIsInAir = OwnerMovementComponent->IsFalling();
		bIsJumping = (MovementMode == EMovementMode::MOVE_Falling && ActorVelocity2D.Z > 0.f);
		bIsFalling = (MovementMode == EMovementMode::MOVE_Falling && ActorVelocity2D.Z < 0.f);

		VelocityRotationAngle = UKismetAnimationLibrary::CalculateDirection(ActorVelocity2D, ActorRotation);
		AccelerationRotationAngle = UKismetAnimationLibrary::CalculateDirection(ActorAcceleration2D, ActorRotation);
		if (ActorAcceleration2D.Size() > 0.f)
		{
			if (UKismetMathLibrary::InRange_FloatFloat(AccelerationRotationAngle, -22.5f, 22.5f))
				LocomotionDirection = ELocomotionDirection::Forward;
			else if (UKismetMathLibrary::InRange_FloatFloat(AccelerationRotationAngle, -67.5f, -22.5f))
				LocomotionDirection = ELocomotionDirection::ForwardLeft;
			else if (UKismetMathLibrary::InRange_FloatFloat(AccelerationRotationAngle, -112.5f, -67.5f))
				LocomotionDirection = ELocomotionDirection::Left;
			else if (UKismetMathLibrary::InRange_FloatFloat(AccelerationRotationAngle, -157.5f, -112.5f))
				LocomotionDirection = ELocomotionDirection::BackwardLeft;
			else if (UKismetMathLibrary::InRange_FloatFloat(AccelerationRotationAngle, 22.5f, 67.5f))
				LocomotionDirection = ELocomotionDirection::ForwardRight;
			else if (UKismetMathLibrary::InRange_FloatFloat(AccelerationRotationAngle, 67.5f, 112.5f))
				LocomotionDirection = ELocomotionDirection::Right;
			else if (UKismetMathLibrary::InRange_FloatFloat(AccelerationRotationAngle, 112.5f, 157.5f))
				LocomotionDirection = ELocomotionDirection::BackwardRight;
			else
				LocomotionDirection = ELocomotionDirection::Backward;
		}
	}
}

bool URHAnimInstance::ReceiveMovementType_Implementation(EMovementType InMovementType)
{
	MovementType = InMovementType;
	return true;
}

bool URHAnimInstance::ReceiveLeftFootDistanceToGround_Implementation(float Distance)
{
	LeftFootToGroundDistance = Distance;
	return true;
}

bool URHAnimInstance::ReceiveRightFootDistanceToGround_Implementation(float Distance)
{
	RightFootToGroundDistance = Distance;
	return true;
}

bool URHAnimInstance::ReceiveJumpPressed_Implementation(bool Pressed)
{
	bIsJumpPressed = Pressed;
	return true;
}

bool URHAnimInstance::ReceiveLockOn_Implementation(bool LockOn)
{
	bIsLockedOn = LockOn;
	return true;
}
