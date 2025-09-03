// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/RHAttributeSet.h"
#include "GameplayEffectExtension.h"

void URHAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		const float LocalDamage = GetDamage();
		SetDamage(0.f);
		if (LocalDamage > 0.f)
		{
			const float NewHealth = FMath::Clamp(GetHealth() - LocalDamage, 0.f, GetMaxHealth());
			SetHealth(NewHealth);
			OnHealthChanged.Broadcast(GetHealth(), GetMaxHealth());
		}
	}

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		OnHealthChanged.Broadcast(GetHealth(), GetMaxHealth());
	}
}
