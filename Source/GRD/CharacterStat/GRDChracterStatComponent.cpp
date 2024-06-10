// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/GRDChracterStatComponent.h"

// Sets default values for this component's properties
UGRDChracterStatComponent::UGRDChracterStatComponent()
{
	MaxHp = 200.0f;
}


// Called when the game starts
void UGRDChracterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	SetHp(MaxHp);
}

float UGRDChracterStatComponent::ApplyDamage(float InDamage)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0.0f, InDamage);

	SetHp(PrevHp - ActualDamage);
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
	}

	return ActualDamage;
}

void UGRDChracterStatComponent::LevelUp()
{
	if (CurrentXp >= MaxHp)
	{
		CurrentXp = 0;
	}

	CurrentLevel++;
}

void UGRDChracterStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, MaxHp);
	OnHpChanged.Broadcast(CurrentHp);
}

