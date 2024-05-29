// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/GRDCharacterBase.h"
#include "Interface/GRDAnimationAttackInterface.h"
#include "GRDCharacterPlayer.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ESkillType : uint8
{
	QSkill UMETA(DisplayName = "QSkill"),
	WSkill UMETA(DisplayName = "WSkill"),
	ESkill UMETA(DisplayName = "ESkill"),
	RSkill UMETA(DisplayName = "RSkill"),
};

UCLASS()
class GRD_API AGRDCharacterPlayer : public AGRDCharacterBase, public IGRDAnimationAttackInterface
{
	GENERATED_BODY()
	
public:
	AGRDCharacterPlayer();

protected:
	bool bCanAction = true;

	virtual void BeginPlay() override;

	void GetCursorLocation(bool& OutHit, FVector& OutLocation) const;

	UFUNCTION(BlueprintCallable, Category = "Action")
	void Move();

	UFUNCTION(BlueprintCallable, Category = "Action")
	void Attack();

	void ProcessComboCommand();
	void ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);

	UFUNCTION(BlueprintCallable, Category = "Action")
	void Skill(UAnimMontage* SkillMontage, ESkillType SkillType);


	void LookCursorPos();

//Atack Hit Section
protected:
	virtual void AttackHitCheck() override;

private:
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UNiagaraSystem> MyCursor;

	UPROPERTY(EditAnywhere, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboActionMontage;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<AActor> QSkillEffect;

	APlayerController* PlayerController;

	
};
