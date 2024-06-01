// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_RSkillEffect.h"
#include "Interface/GRDAnimationAttackInterface.h"

void UAnimNotify_RSkillEffect::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (MeshComp)
	{
		IGRDAnimationAttackInterface* AttackPawn = Cast<IGRDAnimationAttackInterface>(MeshComp->GetOwner());
		if (AttackPawn)
		{
			AttackPawn->SpawnRSkillEffect();
		}
	}
}