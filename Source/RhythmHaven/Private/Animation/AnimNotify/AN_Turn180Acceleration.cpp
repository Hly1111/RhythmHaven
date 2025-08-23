// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_Turn180Acceleration.h"

#include "Interface/RHCharacterActionInterface.h"

void UAN_Turn180Acceleration::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                     const FAnimNotifyEventReference& EventReference)
{
	AActor* Owner = MeshComp->GetOwner();
	if (!Owner)
	{
		return;
	}
	if (Owner->GetClass()->ImplementsInterface(URHCharacterActionInterface::StaticClass()))
		IRHCharacterActionInterface::Execute_Turn180Acceleration(MeshComp->GetOwner());
}
