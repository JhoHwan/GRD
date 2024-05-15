// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/GRDCharacterBase.h"
#include "Interface/GRDCharacterAIInterface.h"
#include "GRDCharacterEnemy.generated.h"

/**
 * 
 */
UCLASS()
class GRD_API AGRDCharacterEnemy : public AGRDCharacterBase, public IGRDCharacterAIInterface
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<class UAnimMontage> AttackMontage;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<class UAnimMontage> DeadMontage;

	void Attack();
	void AttackEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);

	virtual void SetDead() override;

public:
	AGRDCharacterEnemy();

	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed() override;
	virtual void AttackByAI() override;
	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) override;

	FAICharacterAttackFinished OnAttackFinished;
};
