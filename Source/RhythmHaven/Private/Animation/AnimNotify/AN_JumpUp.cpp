// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_JumpUp.h"
#include "Interface/RHCharacterActionInterface.h"

void UAN_JumpUp::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                        const FAnimNotifyEventReference& EventReference)
{
	AActor* Owner = MeshComp->GetOwner();
	if (!Owner)
	{
		return;
	}
	if (Owner->GetClass()->ImplementsInterface(URHCharacterActionInterface::StaticClass()))
	{
		IRHCharacterActionInterface::Execute_JumpUp(Owner);
	}
}
