// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_PlaySound.h"

#include "Kismet/GameplayStatics.h"

void UAN_PlaySound::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                           const FAnimNotifyEventReference& EventReference)
{
	AActor* Owner = MeshComp->GetOwner();
	if (!Owner) return;
	FVector ActorLocation = Owner->GetActorLocation();
	UGameplayStatics::PlaySoundAtLocation(Owner, SoundToPlay, ActorLocation);
}
