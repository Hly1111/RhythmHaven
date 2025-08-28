// Fill out your copyright notice in the Description page of Project Settings.


#include "RHEnemyCharacter.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagsManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/RHHitPayload.h"


// Sets default values
ARHEnemyCharacter::ARHEnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Tags.Add(FName("Enemy"));
}

// Called when the game starts or when spawned
void ARHEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARHEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARHEnemyCharacter::EnemyReceiveDamage_Implementation(FRHHitParams HitData,
	ACharacter* AttackInstigator)
{
	//伤害Effect
	UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(AttackInstigator);
	if (!SourceASC) return;

	URHHitPayload* Payload =  NewObject<URHHitPayload>(GetTransientPackage());
	Payload->PassInData(HitData);

	FGameplayEventData Evt;
	Evt.EventTag = UGameplayTagsManager::Get().RequestGameplayTag(FName("Attack.Hit"));
	Evt.Instigator = AttackInstigator;
	Evt.Target = this;
	Evt.OptionalObject = Payload;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, Evt.EventTag, Evt);
}

bool ARHEnemyCharacter::SetMovementMode_Implementation(EMovementMode Mode)
{
	GetCharacterMovement()->MovementMode = Mode;
	return true;
}

bool ARHEnemyCharacter::IsMovingOnGround_Implementation()
{
	return GetCharacterMovement()->IsMovingOnGround();
}

