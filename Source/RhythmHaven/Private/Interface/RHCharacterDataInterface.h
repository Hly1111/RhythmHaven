// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enum/MovementType.h"
#include "UObject/Interface.h"
#include "RHCharacterDataInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(Blueprintable)
class URHCharacterDataInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RHYTHMHAVEN_API IRHCharacterDataInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Character Data")
	bool ReceiveMovementType(EMovementType MovementType);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Character Data")
	bool ReceiveLeftFootDistanceToGround(float Distance);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Character Data")
	bool ReceiveRightFootDistanceToGround(float Distance);
};
