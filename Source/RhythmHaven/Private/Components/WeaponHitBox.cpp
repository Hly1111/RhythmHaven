// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponHitBox.h"


UWeaponHitBox::UWeaponHitBox()
{
	PrimaryComponentTick.bCanEverTick = false;
	UPrimitiveComponent::SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UWeaponHitBox::ArmHitbox(const FRHHitParams& InParams)
{
	Params = InParams;
	bArmed = true;
	SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SetCollisionObjectType(ECC_Pawn);
}

void UWeaponHitBox::DisarmHitbox()
{
	bArmed = false;
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (OnDisarmHitBox.IsBound())
		OnDisarmHitBox.Broadcast();
}

void UWeaponHitBox::BeginPlay()
{
	Super::BeginPlay();
}


