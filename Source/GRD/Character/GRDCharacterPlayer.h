// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/GRDCharacterBase.h"
#include "Interface/GRDAnimationAttackInterface.h"
#include "GRDCharacterPlayer.generated.h"

/**
 * 
 */
UCLASS()
class GRD_API AGRDCharacterPlayer : public AGRDCharacterBase, public IGRDAnimationAttackInterface
{
	GENERATED_BODY()
	
public:
	AGRDCharacterPlayer();

protected:

	virtual void BeginPlay() override;

	void GetCursorLocation(bool& OutHit, FVector& OutLocation) const;

	UFUNCTION(BlueprintCallable)
	void Move();
	UFUNCTION(BlueprintCallable)
	void Attack();

	void ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);

//Atack Hit Section
protected:
	virtual void AttackHitCheck() override;

private:
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UNiagaraSystem> MyCursor;

	UPROPERTY(EditAnywhere, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboActionMontage;


	APlayerController* PlayerController;

	// IGRDAnimationAttackInterface을(를) 통해 상속됨
};
