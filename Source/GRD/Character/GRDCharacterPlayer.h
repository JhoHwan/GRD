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

//Combo Action Section
protected:
	UFUNCTION(BlueprintCallable, Category = "Action")
	void Attack();

	void ProcessComboCommand();
	void ComboActionBegin();
	void ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
	void SetComboCheckTimer();
	void ComboCheck();


	int32 CurrentCombo = 0;
	FTimerHandle ComboTimerHandle;
	bool HasNextComboCommand = false;

protected:

	UFUNCTION(BlueprintCallable, Category = "Action")
	void Skill(UAnimMontage* SkillMontage, ESkillType SkillType);

	void SpawnRSkillEffect();

	UFUNCTION(BlueprintCallable)
	void LookCursorPos();

//Atack Hit Section
protected:
	virtual void AttackHitCheck() override;

private:
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UNiagaraSystem> MyCursor;

	UPROPERTY(EditAnywhere, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboActionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UGRDComboActionData> ComboActionData;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<AActor> QSkillEffect;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<AActor> RSkillEffect;

	APlayerController* PlayerController;
	
	
};
