// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Enum/OwnerType.h"
#include "RHAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class URHAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	void ApplyInitialGameplayEffects();
	void GiveInitialGameplayAbilities();

	void TryActivateCombo(FGameplayTag ComboTag);
private:
	UPROPERTY()
	TObjectPtr<ACharacter> Owner;

	UPROPERTY()
	EOwnerType OwnerType;
	
	UPROPERTY(EditDefaultsOnly, Category = "Game Effects")
	TArray<TSubclassOf<UGameplayEffect>> InitialGameEffects;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities", BlueprintReadWrite, meta =(AllowPrivateAccess))
	TArray<TSubclassOf<class URHGameplayAbilityBase>> InitialAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effects")
	TSubclassOf<UGameplayEffect> PreInputEffect;
};
