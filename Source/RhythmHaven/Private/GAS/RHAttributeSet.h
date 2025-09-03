// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "RHAttributeSet.generated.h"


#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, NewHealth, float, MaxHealth);
UCLASS()
class URHAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	ATTRIBUTE_ACCESSORS(URHAttributeSet, Health)
	ATTRIBUTE_ACCESSORS(URHAttributeSet, MaxHealth)
	ATTRIBUTE_ACCESSORS(URHAttributeSet, Damage)
	
private:
	UPROPERTY(VisibleAnywhere, Category = "Attribute")
	FGameplayAttributeData Health;

	UPROPERTY(VisibleAnywhere, Category = "Attribute")
	FGameplayAttributeData MaxHealth;

	UPROPERTY(VisibleAnywhere, Category = "Attribute")
	FGameplayAttributeData Damage;

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;
};
