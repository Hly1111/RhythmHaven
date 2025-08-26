

#include "RHPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameplayTagsManager.h"
#include "Components/RHTargeting.h"
#include "GAS/RHAbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Interface/RHCharacterDataInterface.h"
#include "Kismet/KismetMathLibrary.h"

ARHPlayerCharacter::ARHPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(GetRootComponent());

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("View Camera"));
	ViewCamera->SetupAttachment(SpringArm);

	WeaponComponent=CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	WeaponComponent->SetupAttachment(GetMesh(), FName("weapon_rSocket"));

	SpringArm->bUsePawnControlRotation = true;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	Tags.Add(FName("Player"));
}

void ARHPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = GetController<APlayerController>())
	{
		if (UEnhancedInputLocalPlayerSubsystem* LocalPlayerSubsystem = PlayerController->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (GameplayInputContext)
			{
				LocalPlayerSubsystem->RemoveMappingContext(GameplayInputContext);
				LocalPlayerSubsystem->AddMappingContext(GameplayInputContext, 0);
			}
		}
	}
}

void ARHPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateFootStep(FName("l_foot_sound"), LeftFootSound, bLeftFootStepPlayed, LeftFootDistanceToGround);
	UpdateFootStep(FName("r_foot_sound"), RightFootSound, bRightFootStepPlayed, RightFootDistanceToGround);

	CalculateCameraAngularDifference();

	if (bIsLockedOn && !bPressedJump && !GetMesh()->GetAnimInstance()->WasAnimNotifyStateActiveInAnyState(NoRotateCharacterNotifyState))
	{
		RotateForwardVectorToEnemy();

		PreviousEnemy = TargettingComponent->GetCurrentEnemy();
		bResetLockOnRotation = false;
	}
	else
	{
		if (!GetMesh()->GetAnimInstance()->IsAnyMontagePlaying())
		{
			if (!bResetLockOnRotation)
			{
				bResetLockOnRotation = true;
				TargettingComponent->ResetCurrentEnemies();
			}
		}
	}
}

void ARHPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MoveAction)
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARHPlayerCharacter::HandleMoveAction);
		}
		if (LookAction)
		{
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARHPlayerCharacter::HandleLookAction);
		}

		if (SprintAction)
		{
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ARHPlayerCharacter::HandleSprintStart);
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ARHPlayerCharacter::HandleSprintStop);
		}

		if (JumpAction)
		{
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started,this,&ARHPlayerCharacter::HandleJumpStart);
			EnhancedInputComponent->BindAction(JumpAction,ETriggerEvent::Completed,this,&ARHPlayerCharacter::HandleJumpRelease);
		}

		if (MeleeAttackAction)
		{
			EnhancedInputComponent->BindAction(MeleeAttackAction, ETriggerEvent::Started,this, &ARHPlayerCharacter::HandleMeleeAttack);
		}

		if (LockOnAction)
		{
			EnhancedInputComponent->BindAction(LockOnAction,ETriggerEvent::Started,this, &ARHPlayerCharacter::HandleLockOn);
		}

		if (LockOnSwitchAction)
		{
			EnhancedInputComponent->BindAction(LockOnSwitchAction,ETriggerEvent::Started,this, &ARHPlayerCharacter::HandleLockOnSwitch);
		}
	}
}

FVector ARHPlayerCharacter::GetLookForwardDirection() const
{
	return ViewCamera->GetForwardVector();
}

FVector ARHPlayerCharacter::GetLookRightDirection() const
{
	return ViewCamera->GetRightVector();
}

FVector ARHPlayerCharacter::GetFaceForwardDirection() const
{
	return FVector::CrossProduct(GetLookRightDirection(), FVector::UpVector);
}

void ARHPlayerCharacter::CalculateCameraAngularDifference()
{
	if(GetCharacterMovement()->GetLastInputVector().Length()>0 && !bIsLockedOn)
	{
		YawDifference = FMath::FindDeltaAngleDegrees(GetBaseAimRotation().Yaw, GetActorRotation().Yaw);
		float OutValue = FMath::GetMappedRangeValueClamped(TRange<float>(55.f, 180.f), TRange<float>(0.2f, 0.6f), FMath::Abs(YawDifference));
		if (UKismetMathLibrary::InRange_FloatFloat(YawDifference, 55.f, 145.f, true, true))
		{
			AddControllerYawInput(OutValue);
		}
		else if (UKismetMathLibrary::InRange_FloatFloat(YawDifference, -145.f, -55.f, true, true))
		{
			AddControllerYawInput(-OutValue);
		}
	}
}

void ARHPlayerCharacter::RotateForwardVectorToEnemy()
{
	AActor* CurrentEnemy = TargettingComponent->GetCurrentEnemy();
	if (IsValid(CurrentEnemy))
	{
		FRotator CurrentLookAtRotation = GetActorRotation();
		FRotator LookAtTargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CurrentEnemy->GetActorLocation());
		float Yaw = UKismetMathLibrary::RInterpTo(CurrentLookAtRotation, LookAtTargetRotation, GetWorld()->GetDeltaSeconds(), 10).Yaw;
		SetActorRotation(FRotator(0, Yaw, 0));
	}
}

void ARHPlayerCharacter::HandleMoveAction(const FInputActionValue& Value)
{
	FVector2d MovementValue = Value.Get<FVector2d>();
	MovementValue.Normalize();

	const float ScaleValueX = GetCharacterMovement()->IsMovingOnGround()? MovementValue.X : MovementValue.X * 1.2f;
	const float ScaleValueY = GetCharacterMovement()->IsMovingOnGround()? MovementValue.Y : MovementValue.Y * 1.2f;
	AddMovementInput(GetFaceForwardDirection(), ScaleValueY);
	AddMovementInput(GetLookRightDirection(), ScaleValueX);
}

void ARHPlayerCharacter::HandleLookAction(const FInputActionValue& Value)
{
	FVector2d LookValue = Value.Get<FVector2d>();
	
	AddControllerPitchInput(LookValue.Y);
	AddControllerYawInput(LookValue.X);
}

void ARHPlayerCharacter::HandleSprintStart()
{
	if (this->GetClass()->ImplementsInterface(URHCharacterActionInterface::StaticClass()))
	{
		Execute_ChangeMovementType(this, EMovementType::Sprint, 1000.f);
	}
}

void ARHPlayerCharacter::HandleSprintStop()
{
	if (this->GetClass()->ImplementsInterface(URHCharacterActionInterface::StaticClass()))
	{
		Execute_ChangeMovementType(this, EMovementType::Run, 800.f);
	}
}

void ARHPlayerCharacter::HandleJumpStart()
{
	if (!bIsLockedOn)
	{
		StopAnimMontage(GetMesh()->GetAnimInstance()->GetCurrentActiveMontage());
		Execute_ReceiveJumpPressed(GetMesh()->GetAnimInstance(), true);
	}
}

void ARHPlayerCharacter::HandleJumpRelease()
{
	//Execute_ReceiveJumpPressed(GetMesh()->GetAnimInstance(), false);
}

void ARHPlayerCharacter::Landed(const FHitResult& Hit)
{
	Execute_ReceiveJumpPressed(GetMesh()->GetAnimInstance(), false);
	GetCharacterMovement()->MaxAcceleration = 1900.f;
	GetCharacterMovement()->GravityScale = 6.f;
	GetCharacterMovement()->MaxWalkSpeed = bIsLockedOn? 150.f : 800.f;
	HandleSprintStop();
}

void ARHPlayerCharacter::HandleMeleeAttack()
{
	ASC->TryActivateCombo(GetMeleeAttackTag());
}

#if WITH_EDITOR
#define RH_DRAW_FOOT_LINE(World, Start, End, bHit, Duration)\
DrawDebugLine((World), (Start), (End), (bHit) ? FColor::Red : FColor::Green, false, (Duration), 0, 0.15f);
#else
#define RH_DRAW_FOOT_LINE(World, Start, End, bHit, Duration)
#endif

void ARHPlayerCharacter::HandleLockOn()
{
	if (!bIsLockedOn)
	{
		bIsLockedOn = true;
		IRHCharacterDataInterface::Execute_ReceiveLockOn(GetMesh()->GetAnimInstance(), bIsLockedOn);
		GetCharacterMovement()->MaxAcceleration = 200.f;
		Execute_ChangeMovementType(this, EMovementType::Walk, 150);
		TargettingComponent->FindClosestEnemy();
	}
	else
	{
		HandleLockOff();
	}
}

void ARHPlayerCharacter::HandleLockOff()
{
	bIsLockedOn = false;
	IRHCharacterDataInterface::Execute_ReceiveLockOn(GetMesh()->GetAnimInstance(), bIsLockedOn);
	GetCharacterMovement()->MaxAcceleration = 1900.f;
	Execute_ChangeMovementType(this, EMovementType::Run, 800);
}

void ARHPlayerCharacter::HandleLockOnSwitch()
{
	if (!bIsLockedOn) return;
	TargettingComponent->FindClosestEnemy();
}

void ARHPlayerCharacter::UpdateFootStep(FName SocketName,  USoundBase* FootSound, bool& bIsStepPlayed, float& DistanceToGround) const
{
	const FVector Start = GetMesh()->GetSocketLocation(SocketName);
	const FVector End = Start + (-1000.f) * GetActorUpVector();

	FHitResult OutHit;
	const bool bHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_WorldStatic);

	RH_DRAW_FOOT_LINE(GetWorld(), Start, End, bHit, 0.1f);

	if (bHit)
	{
		DistanceToGround = OutHit.Distance;

		if (SocketName == TEXT("l_foot_sound"))
		{
			IRHCharacterDataInterface::Execute_ReceiveLeftFootDistanceToGround(GetMesh()->GetAnimInstance(), DistanceToGround);
		}
		else if (SocketName == TEXT("r_foot_sound"))
		{
			IRHCharacterDataInterface::Execute_ReceiveRightFootDistanceToGround(GetMesh()->GetAnimInstance(), DistanceToGround);
		}
		
		if (DistanceToGround < 4.f)
		{
			if (!bIsStepPlayed)
			{
				if (FootSound)
				{
					UGameplayStatics::PlaySoundAtLocation(this, FootSound, OutHit.Location);
				}
				bIsStepPlayed = true;
			}
		}
		else if (DistanceToGround >= 10.f)
		{
			bIsStepPlayed = false;
		}
	}
}

bool ARHPlayerCharacter::ChangeMovementType_Implementation(EMovementType MovementType, float MaxWalkSpeed)
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		if (AnimInstance ->GetClass()->ImplementsInterface(URHCharacterDataInterface::StaticClass()))
		{
			Execute_ReceiveMovementType(AnimInstance, MovementType);
			GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
			return true;
		}
	}
	return false;
}

void ARHPlayerCharacter::RunStartAcceleration_Implementation()
{
	Accelerate(1.2f);
}

void ARHPlayerCharacter::Turn180Acceleration_Implementation()
{
	Accelerate(0.9f);
}

void ARHPlayerCharacter::RecoverSpeed_Implementation()
{
	Accelerate(0.6f);
}

void ARHPlayerCharacter::Accelerate(float Multiplier) const
{
	FVector Acceleration = GetCharacterMovement()->GetCurrentAcceleration();
	if (Acceleration != FVector::ZeroVector)
	{
		GetCharacterMovement()->Velocity = Multiplier * Acceleration;
	}
}

void ARHPlayerCharacter::JumpUp_Implementation()
{
	GetCharacterMovement()->MaxWalkSpeed = 800.f;
	GetCharacterMovement()->JumpZVelocity = 4200.f;
	GetCharacterMovement()->GravityScale = 12.f;
	GetCharacterMovement()->MaxAcceleration = 3000.f;
	Jump();
}

FGameplayTag ARHPlayerCharacter::GetMeleeAttackTag()
{
	return UGameplayTagsManager::Get().RequestGameplayTag(FName("Attack.MeleeAttack"));
}



