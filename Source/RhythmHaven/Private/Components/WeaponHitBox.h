// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RHHitParams.h"
#include "Components/BoxComponent.h"
#include "WeaponHitBox.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDisarmHitBox);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RHYTHMHAVEN_API UWeaponHitBox : public UBoxComponent
{
	GENERATED_BODY()

public:
	UWeaponHitBox();
	
	void ArmHitbox(const FRHHitParams& InParams);
	void DisarmHitbox();

	UFUNCTION(BlueprintCallable)
	const FRHHitParams& GetParams() const { return Params; }
	
	bool IsArmed() const { return bArmed; }

	UPROPERTY(BlueprintAssignable)
	FOnDisarmHitBox OnDisarmHitBox;
protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY()
	FRHHitParams Params;
	
	bool bArmed = false;
};
