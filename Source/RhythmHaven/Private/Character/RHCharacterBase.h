// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "RHCharacterBase.generated.h"

UCLASS(Abstract)
class RHYTHMHAVEN_API ARHCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ARHCharacterBase();

	class URHHit* GetHitComponent() const;
	class URHTargeting* GetTargetingComponent() const;
	class URHMotionWarping* GetMotionWarpingComponent() const;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
 
protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Components")
	TObjectPtr<class URHAbilitySystemComponent> ASC;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Components")
	TObjectPtr<class URHAttributeSet> AttributeSet;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Character Components")
	TObjectPtr<URHHit> HitComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Character Components")
	TObjectPtr<class URHTargeting> TargetingComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Character Components")
	TObjectPtr<class URHMotionWarping> MotionWarpingComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	TSubclassOf<UAnimInstance> LinkAnimLayerClass;
};
