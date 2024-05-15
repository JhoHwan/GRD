// Fill out your copyright notice in the Description page of Project Settings.


#include "GRDCharacterEnemy.h"
#include "AI/GRDAIController.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "AI/GRDAIController.h"
#include "GameFramework/CharacterMovementComponent.h"


AGRDCharacterEnemy::AGRDCharacterEnemy()
{

}

float AGRDCharacterEnemy::GetAIAttackRange()
{
	return 100.0f;
}

float AGRDCharacterEnemy::GetAITurnSpeed()
{
	return 3.0f;
}

void AGRDCharacterEnemy::Attack()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	GetCharacterMovement()->StopActiveMovement();
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	AnimInstance->Montage_Play(AttackMontage);
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AGRDCharacterEnemy::AttackEnd);

	AnimInstance->Montage_SetEndDelegate(EndDelegate, AttackMontage);
}

void AGRDCharacterEnemy::AttackEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	OnAttackFinished.ExecuteIfBound();
}

void AGRDCharacterEnemy::SetDead()
{
	Super::SetDead();
	
	AGRDAIController* AIController = Cast<AGRDAIController>(GetController());
	if (AIController)
	{
		AIController->StopAI();
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(DeadMontage);
	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			Destroy();
		}
	), 3.0f, false);

}

void AGRDCharacterEnemy::AttackByAI()
{
	Attack();
	
}

void AGRDCharacterEnemy::SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}
