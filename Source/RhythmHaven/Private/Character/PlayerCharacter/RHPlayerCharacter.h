// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/RHCharacterBase.h"
#include "InputActionValue.h"
#include "Interface/RHCharacterDataInterface.h"
#include "RHPlayerCharacter.generated.h"

UCLASS(Abstract)
class RHYTHMHAVEN_API ARHPlayerCharacter : public ARHCharacterBase, public IRHCharacterActionInterface, public IRHCharacterDataInterface
{
	GENERATED_BODY()

public:
	ARHPlayerCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	TObjectPtr<class UCameraComponent> ViewCamera;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	TObjectPtr<USkeletalMeshComponent> WeaponComponent;
	
	FVector GetLookForwardDirection() const;
	FVector GetLookRightDirection() const;
	FVector GetFaceForwardDirection() const;

	
	/*Input*/
private:
	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	TObjectPtr<class UInputMappingContext> GameplayInputContext;
	
	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	TObjectPtr<class UInputAction> MoveAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	TObjectPtr<UInputAction> LookAction;

	void HandleMoveAction(const FInputActionValue& Value);
	void HandleLookAction(const FInputActionValue& Value);

	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	TObjectPtr<UInputAction> SprintAction;

	void HandleSprintStart(const FInputActionValue& Value);
	void HandleSprintStop(const FInputActionValue& Value);
	
	/*Foot Step*/
	void UpdateFootStep(FName SocketName,  USoundBase* FootSound, bool& bIsStepPlayed, float& DistanceToGround) const;
	
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	TObjectPtr<USoundBase> LeftFootSound;
	
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	TObjectPtr<USoundBase> RightFootSound;
	
	bool bLeftFootStepPlayed;
	bool bRightFootStepPlayed;
	float LeftFootDistanceToGround;
	float RightFootDistanceToGround;

private:
	virtual bool ChangeMovementType_Implementation(EMovementType MovementType, float MaxWalkSpeed) override;
	virtual void RunStartAcceleration_Implementation() override;
	virtual void Turn180Acceleration_Implementation() override;
	virtual void RecoverSpeed_Implementation() override;
	void Accelerate(float Multiplier) const;
};
