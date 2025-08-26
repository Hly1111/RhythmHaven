// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RHTargeting.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RHYTHMHAVEN_API URHTargeting : public UActorComponent
{
	GENERATED_BODY()

public:
	URHTargeting();
	void FindClosestEnemy(float DetectRange = 1500.f);
	AActor* GetCurrentEnemy() const;
	void ResetCurrentEnemies();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TObjectPtr<AActor> Owner;
	
	UPROPERTY()
	TArray<TObjectPtr<AActor>> CurrentEnemies = {nullptr, nullptr, nullptr};
	UPROPERTY()
	TArray<TObjectPtr<AActor>> AllEnemiesInRange;

	TArray<AActor*> FindEnemyWithinRange(float DetectRange) const;
	AActor* WhichEnemyToUse() const;
	void SetWhichEnemy(AActor* InEnemy);
};
