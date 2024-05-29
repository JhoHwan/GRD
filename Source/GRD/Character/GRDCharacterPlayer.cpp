// Fill out your copyright notice in the Description page of Project Settings.


#include "GRDCharacterPlayer.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Physics/GRDCollision.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"

AGRDCharacterPlayer::AGRDCharacterPlayer()
{
}

void AGRDCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();
	PlayerController = Cast<APlayerController>(GetController());
}

void AGRDCharacterPlayer::GetCursorLocation(bool& OutHit, FVector& OutLocation) const
{
	FHitResult Hit;
	PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);

	OutHit = Hit.bBlockingHit;
	OutLocation = Hit.Location;
}

void AGRDCharacterPlayer::Move()
{
	bool Hit;
	FVector Location;
	GetCursorLocation(Hit, Location);

	if (Hit)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), MyCursor, Location, FRotator(0, 0, 0), FVector(2.0f, 2.0f, 2.0f));
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(PlayerController, Location);
	}
}

void AGRDCharacterPlayer::Attack()
{
	if (!bCanAction)
	{
		return;
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	GetCharacterMovement()->StopActiveMovement();
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	bCanAction = false;

	LookCursorPos();

	ProcessComboCommand();

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AGRDCharacterPlayer::ComboActionEnd);

	AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboActionMontage);
}

void AGRDCharacterPlayer::Skill(UAnimMontage* SkillMontage, ESkillType SkillType)
{
	if (!bCanAction)
	{
		return;
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	GetCharacterMovement()->StopActiveMovement();
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	bCanAction = false;
	AnimInstance->Montage_Play(SkillMontage);
	FActorSpawnParameters ActorSpawnParameters;

	switch (SkillType)
	{
	case ESkillType::QSkill:
		LookCursorPos();
		GetWorld()->SpawnActor<AActor>(QSkillEffect, GetActorLocation(), GetActorRotation(), ActorSpawnParameters);
		
		break;
	case ESkillType::WSkill:
		break;
	case ESkillType::ESkill:
		break;
	case ESkillType::RSkill:
		break;
	default:
		break;
	}


	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AGRDCharacterPlayer::ComboActionEnd);

	AnimInstance->Montage_SetEndDelegate(EndDelegate, SkillMontage);
}


void AGRDCharacterPlayer::ProcessComboCommand()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ComboActionMontage);
}

void AGRDCharacterPlayer::ComboActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	bCanAction = true;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void AGRDCharacterPlayer::LookCursorPos()
{
	bool Hit;
	FVector Location;
	GetCursorLocation(Hit, Location);

	if (Hit)
	{
		FVector ToCursor = Location - GetActorLocation();
		ToCursor.Z = 0;
		FRotator Rotator = ToCursor.Rotation();
		
		SetActorRotation(Rotator);
	}

}

void AGRDCharacterPlayer::AttackHitCheck()
{
	TArray<FHitResult> OutHitResults;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	const float AttackRange = 900.0f;
	const float AttackRadius = 250.0f;
	const float AttackDamage = 30.0f;
	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + GetActorForwardVector() * AttackRange;

	bool HitDetected = GetWorld()->SweepMultiByChannel(OutHitResults, Start, End, FQuat::Identity, CCHANNEL_GRDACTION, FCollisionShape::MakeSphere(AttackRadius), Params);
	if (HitDetected)
	{
		FDamageEvent DamageEvent;
		for (auto& OutHitResult : OutHitResults)
		{
			OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
		} 
	}

/*
#if ENABLE_DRAW_DEBUG

	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	float CapsuleHalfHeight = AttackRange * 0.5f;
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.0f);

#endif
*/
}
