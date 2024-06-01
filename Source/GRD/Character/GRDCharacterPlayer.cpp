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
#include "Character/GRDComboActionData.h"

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
	FVector vector = GetActorLocation();
	vector.Z += 300;

	switch (SkillType)
	{
	case ESkillType::QSkill:
		LookCursorPos();
		GetWorld()->SpawnActor<AActor>(QSkillEffect, vector, GetActorRotation(), ActorSpawnParameters);
		
		break;
	case ESkillType::WSkill:
		break;
	case ESkillType::ESkill:
		break;
	case ESkillType::RSkill:
		LookCursorPos();

		break;
	default:
		break;
	}


	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AGRDCharacterPlayer::ComboActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, SkillMontage);

}

void AGRDCharacterPlayer::Attack()
{
	ProcessComboCommand();
}

void AGRDCharacterPlayer::ProcessComboCommand()
{
	if (CurrentCombo == 0 && bCanAction == true)
	{
		ComboActionBegin();
		return;
	}

	if (!ComboTimerHandle.IsValid())
	{
		HasNextComboCommand = false;
	}
	else
	{
		HasNextComboCommand = true;
	}
}

void AGRDCharacterPlayer::ComboActionBegin()
{
	bCanAction = false;
	//Combo Status
	CurrentCombo = 1;

	LookCursorPos();

	//Movement Setting
	GetCharacterMovement()->StopActiveMovement();
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	const float AttackSpeedRate = 1.0f;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ComboActionMontage, AttackSpeedRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AGRDCharacterPlayer::ComboActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboActionMontage);

	ComboTimerHandle.Invalidate();
	SetComboCheckTimer();
}

void AGRDCharacterPlayer::ComboActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	ensure(CurrentCombo != 0);
	bCanAction = true;
	CurrentCombo = 0;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void AGRDCharacterPlayer::SetComboCheckTimer()
{
	int32 ComboIndex = CurrentCombo - 1;
	ensure(ComboActionData->EffectiveFrameCount.IsValidIndex(ComboIndex));

	const float AttackSpeedRate = 1.0f;
	float ComboEffectiveTime = (ComboActionData->EffectiveFrameCount[ComboIndex] / ComboActionData->FrameRate) / AttackSpeedRate;
	if (ComboEffectiveTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &AGRDCharacterPlayer::ComboCheck, ComboEffectiveTime, false);
	}
}

void AGRDCharacterPlayer::ComboCheck()
{
	ComboTimerHandle.Invalidate();
	if (HasNextComboCommand)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboActionData->MaxComboCount);
		UE_LOG(LogTemp, Log, TEXT("%d"), CurrentCombo);

		FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboActionData->MontageSectionNamePrefix, CurrentCombo);
		AnimInstance->Montage_JumpToSection(NextSection, ComboActionMontage);
		SetComboCheckTimer();
		HasNextComboCommand = false;
	}
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
