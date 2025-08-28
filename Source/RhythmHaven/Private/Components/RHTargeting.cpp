// Fill out your copyright notice in the Description page of Project Settings.


#include "RHTargeting.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values for this component's properties
URHTargeting::URHTargeting()
{
	AllEnemiesInRange.Reserve(10);
}

void URHTargeting::FindClosestEnemy(float DetectRange)
{
	if (!IsValid(Owner))
	{
		ResetCurrentEnemies();
		AllEnemiesInRange.Reset();
		return;
	}

	auto TryFillSlot = [&](int32 Slot)->bool
	{
		auto Found = FindEnemyWithinRange(1500.f);
		if (Found.Num() > 0)
		{
			AllEnemiesInRange = MoveTemp(Found);
			CurrentEnemies[Slot] = AllEnemiesInRange[0];
			return true;
		}
		return false;
	};
	
	if (!CurrentEnemies[0])
	{
		if (!TryFillSlot(0)) return;
	}
	else if (!CurrentEnemies[1])
	{
		if (!TryFillSlot(1)) return;
	}
	else if (!CurrentEnemies[2])
	{
		auto Enemies = FindEnemyWithinRange(1500.f);
		if (Enemies.Num() >= 1)
		{
			AllEnemiesInRange = Enemies;
			CurrentEnemies[2] = AllEnemiesInRange[0];
		}
		else
		{
			ResetCurrentEnemies();
			if (!TryFillSlot(0)) return;
		}
	}
	else
	{
		ResetCurrentEnemies();
		FindClosestEnemy();
		return;
	}
	
	AActor* Chosen = WhichEnemyToUse();
	if (!Chosen && AllEnemiesInRange.Num() > 0)
	{
		SetWhichEnemy(AllEnemiesInRange[0]);
		Chosen = WhichEnemyToUse();
	}
	
	const FVector OwnerLoc = GetOwner()->GetActorLocation();
	float ChosenD2 = IsValid(Chosen)
	? FVector::DistSquared(OwnerLoc, Chosen->GetActorLocation())
	: TNumericLimits<float>::Max();

	for (AActor* Enemy : AllEnemiesInRange)
	{
		if (!IsValid(Enemy)) continue;
		const FVector EnemyLoc = Enemy->GetActorLocation();
		const float EnemyD2 = FVector::DistSquared(OwnerLoc, EnemyLoc);
		if (!IsValid(Chosen) || EnemyD2 < ChosenD2)
		{
			SetWhichEnemy(Enemy);
			Chosen = Enemy;
			ChosenD2 = EnemyD2;
		}
	}
}

AActor* URHTargeting::GetCurrentEnemy() const
{
	return WhichEnemyToUse();
}

// Called when the game starts
void URHTargeting::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
}

TArray<AActor*> URHTargeting::FindEnemyWithinRange(float DetectRange) const
{
	TArray<AActor*> Enemies;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add( UEngineTypes::ConvertToObjectType(ECC_Pawn));
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Owner);
	for (const auto& CurrentEnemy : CurrentEnemies)
	{
		if (IsValid(CurrentEnemy)) 
		{
			ActorsToIgnore.Add(CurrentEnemy);
		}
	}
	UKismetSystemLibrary::SphereOverlapActors(
		GetOwner(), 
		GetOwner()->GetActorLocation(), 
		DetectRange, 
		ObjectTypes, 
		nullptr,
		ActorsToIgnore, 
		Enemies);
	return Enemies;
}

AActor* URHTargeting::WhichEnemyToUse() const
{
	for (int i = CurrentEnemies.Num() - 1; i >= 0; --i)
	{
		if (IsValid(CurrentEnemies[i]))
		{
			return CurrentEnemies[i];
		}
	}
	return nullptr;
}

void URHTargeting::SetWhichEnemy(AActor* InEnemy)
{
	if (!IsValid(InEnemy))
	{
		return;
	}
	for (int i = CurrentEnemies.Num() - 1; i >= 0; --i)
	{
		if (IsValid(CurrentEnemies[i]))
		{
			CurrentEnemies[i] = InEnemy;
			break;
		}
	}
	CurrentEnemies[0] = InEnemy;
}

void URHTargeting::ResetCurrentEnemies()
{
	for (int i = 0; i < CurrentEnemies.Num(); ++i)
	{
		CurrentEnemies[i] = nullptr;
	}
}

