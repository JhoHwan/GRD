// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GRDCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AGRDCharacterBase::AGRDCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

float AGRDCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	return DamageAmount;
}

void AGRDCharacterBase::SetDead()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	GetMesh()->GetAnimInstance()->StopAllMontages(0.0f);

	SetActorEnableCollision(false);
}