// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GRDChracterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedChanged, float /*CurrentHp*/)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GRD_API UGRDChracterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGRDChracterStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	FOnHpZeroDelegate OnHpZero;
	FOnHpChangedChanged OnHpChanged;

	FORCEINLINE float GetMaxHp() const { return MaxHp; }
	FORCEINLINE float GetCurrentHp() { return CurrentHp; }
	float ApplyDamage(float InDamage);

	FORCEINLINE uint32 GetMaxLevel() const { return MaxLevel; }
	FORCEINLINE uint32 GetCurrentLevel() const { return CurrentLevel; }
	void LevelUp();

protected:
	void SetHp(float NewHp);

	UPROPERTY(VisibleInstanceOnly, Category = "Stat")	
	float MaxHp;

	UPROPERTY(Transient ,VisibleInstanceOnly, Category = "Stat")
	float CurrentHp;


	UPROPERTY(VisibleInstanceOnly, Category = "Stat")
	uint32 MaxLevel;

	UPROPERTY(Transient ,VisibleInstanceOnly, Category = "Stat")
	uint32 CurrentLevel;


	UPROPERTY(VisibleInstanceOnly, Category = "Stat")
	uint32 MaxXp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Stat")
	uint32 CurrentXp;


	
};
