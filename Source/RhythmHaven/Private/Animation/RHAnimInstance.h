// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Enum/LocomotionDirection.h"
#include "Enum/MovementType.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interface/RHCharacterDataInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "RHAnimInstance.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class RHYTHMHAVEN_API URHAnimInstance : public UAnimInstance, public IRHCharacterDataInterface
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

private:
	UPROPERTY()
	TObjectPtr<ACharacter> OwnerCharacter;

	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> OwnerMovementComponent;

	float ActorRotationYaw;
	float CachedActorRotationYaw;
	FRotator ActorRotation;
	float ActorLeanAngle;
	
	FVector ActorVelocity2D;
	float AccelerationRotationAngle;
	bool bIsAccelerating;
	EMovementMode MovementMode;
	bool bIsJumping;
	bool bIsFalling;

	bool bIsLockedOn;

	ELocomotionDirection LocomotionDirection;
	EMovementType MovementType = EMovementType::Run;

	float LeftFootToGroundDistance;
	float RightFootToGroundDistance;

	UFUNCTION(BlueprintCallable, Category = "Animation Data", meta = (BlueprintThreadSafe))
	bool IsAccelerating() const { return bIsAccelerating; }

	UFUNCTION(BlueprintCallable, Category = "Animation Data", meta = (BlueprintThreadSafe))
	bool IsNotAccelerating() const { return !bIsAccelerating; }

	UFUNCTION(BlueprintCallable, Category = "Animation Data", meta = (BlueprintThreadSafe))
	EMovementType GetMovementType() const { return MovementType; }

	UFUNCTION(BlueprintCallable, Category = "Animation Data", meta = (BlueprintThreadSafe))
	float GetCurrentVelocityValue() const { return ActorVelocity2D.Length(); }

	UFUNCTION(BlueprintCallable, Category = "Animation Data", meta = (BlueprintThreadSafe))
	bool IsAtMaxWalkSpeed() const { return FMath::IsNearlyEqual(GetCurrentVelocityValue(), GetMaxWalkSpeed()); }

	UFUNCTION(BlueprintCallable, Category = "Animation Data")
	float GetMaxWalkSpeed() const { return OwnerMovementComponent ? OwnerMovementComponent->GetMaxSpeed() : 0.f; }

	UFUNCTION(BlueprintCallable, Category = "Animation Data")
	float GetActorLeanAngle() const { return ActorLeanAngle; }

	UFUNCTION(BlueprintCallable, Category = "Animation Data")
	bool IsTurnLeft180() const { return UKismetMathLibrary::InRange_FloatFloat(AccelerationRotationAngle, -180.f, -110.f, true, true); }

	UFUNCTION(BlueprintCallable, Category = "Animation Data")
	bool IsTurnRight180() const { return UKismetMathLibrary::InRange_FloatFloat(AccelerationRotationAngle, 110.f, 180.f, true, true); }

	UFUNCTION(BlueprintCallable, Category = "Animation Data", meta = (BlueprintThreadSafe))
	bool IsTurn180() const { return IsTurnLeft180() || IsTurnRight180(); }

	UFUNCTION(BlueprintCallable, Category = "Animation Data", meta = (BlueprintThreadSafe))
	bool IsLockedOn() const { return bIsLockedOn; }

	virtual bool ReceiveMovementType_Implementation(EMovementType InMovementType) override;
	virtual bool ReceiveLeftFootDistanceToGround_Implementation(float Distance) override;
	virtual bool ReceiveRightFootDistanceToGround_Implementation(float Distance) override;
};
