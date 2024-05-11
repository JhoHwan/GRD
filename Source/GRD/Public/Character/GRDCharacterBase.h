// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include <GRD/GRD.h>
#include "GRDCharacterBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterDiedDelegate, AGRDCharacterBase*, character);


UCLASS()
class GRD_API AGRDCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGRDCharacterBase(const class FObjectInitializer& ObjectInitalizer);

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;


	UPROPERTY(BlueprintAssignable, Category = "GRD|Character")
	FCharacterDiedDelegate OnCharacterDied;

	UFUNCTION(BlueprintCallable, Category = "GRD|Character")
	virtual bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category = "GRD|Character")
	virtual int32 GetAbilityLevel(GRDAbilityID AbilityID) const;

	virtual void RemoveCharacterAbilities();

	virtual void Die();

	UFUNCTION(BlueprintCallable, Category = "GRD|Character")
	virtual void FinishDying();

	UFUNCTION(BlueprintCallable, Category = "GRD|Character|Attributes")
	float GetCharacterLevel() const;

	UFUNCTION(BlueprintCallable, Category = "GRD|Character|Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "GRD|Character|Attributes")
	float GetMaxHealth() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TWeakObjectPtr<class UCharacterAbilitySystemComponent> AbilitySystemComponent;
	TWeakObjectPtr<class UCharacterAttributeSetBase> AttributeSetBase;

	FGameplayTag DeadTag;
	FGameplayTag EffectRemoveOnDeathTag;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GRD|Character")
	FText CharacterName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GRD|Animation")
	UAnimMontage* DeathMontage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Demo|Abilities")
	TArray < TSubclassOf<class UCharacterGameplayAbility> > CharacterAbilities;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Demo|Abilities")
	TSubclassOf<class UGameplayEffect> DefaultAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Demo|Abilities")
	TArray < TSubclassOf<class UGameplayEffect> > StartupEffects;

	virtual void AddCharacterAbilities();

	virtual void InitializeAttributes();

	virtual void AddStartupEffects();

	virtual void SetLevel(float Level);

	virtual void SetHealth(float Health);
};
