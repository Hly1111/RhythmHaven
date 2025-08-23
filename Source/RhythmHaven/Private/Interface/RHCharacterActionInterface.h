// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enum/MovementType.h"
#include "UObject/Interface.h"
#include "RHCharacterActionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType)
class URHCharacterActionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RHYTHMHAVEN_API IRHCharacterActionInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Character Action")
	bool ChangeMovementType(EMovementType MovementType, float MaxWalkSpeed);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Character Action")
	void RunStartAcceleration();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Character Action")
	void Turn180Acceleration();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Character Action")
	void RecoverSpeed();
};
