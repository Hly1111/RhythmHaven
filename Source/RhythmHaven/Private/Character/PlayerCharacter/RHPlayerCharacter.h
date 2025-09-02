// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Character/RHCharacterBase.h"
#include "InputActionValue.h"
#include "RHPlayerCharacter.generated.h"

UCLASS(Abstract)
class RHYTHMHAVEN_API ARHPlayerCharacter : public ARHCharacterBase
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon",  meta = (AllowPrivateAccess))
	TObjectPtr<class UWeaponHitBox> WeaponBox; 
	
	FVector GetLookForwardDirection() const;
	FVector GetLookRightDirection() const;
	FVector GetFaceForwardDirection() const;
	void CalculateCameraAngularDifference();
	float YawDifference;

	bool bIsLockedOn;	
	bool bResetLockOnRotation;
	
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	TSubclassOf<UAnimNotifyState> NoRotateCharacterNotifyState;

	void RotateForwardVectorToEnemy();
	
	UPROPERTY()
	TObjectPtr<AActor> PreviousEnemy;
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

	void HandleSprintStart();
	void HandleSprintStop();

	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	TObjectPtr<UInputAction> JumpAction;

	void HandleJumpStart();
	void HandleJumpRelease();
	virtual void Landed(const FHitResult& Hit) override;

	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	TObjectPtr<UInputAction> LockOnAction;

	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	TObjectPtr<UInputAction> LockOnSwitchAction;

	void HandleLockOn();
	void HandleLockOff();
	void HandleLockOnSwitch();

	UPROPERTY(EditDefaultsOnly, Category = "Enhanced Input")
	TObjectPtr<UInputAction> MeleeAttackAction;

	void HandleMeleeAttack();
	
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
	virtual void JumpUp_Implementation() override;
	virtual void PlayAttackSound_Implementation(USoundBase* SoundToPlay, float StartTime) override;
	virtual void ShakeCamera_Implementation(TSubclassOf<class UCameraShakeBase> CameraShakeClass, float Scale) override;
	virtual void EnemyReceiveDamage_Implementation(FRHHitParams HitData, ACharacter* AttackInstigator) override;
	virtual bool IsMovingOnGround_Implementation() override;

	/* TAGS */
	static FGameplayTag GetMeleeAttackTag();
};
