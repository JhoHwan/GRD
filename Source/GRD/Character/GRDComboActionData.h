// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GRDComboActionData.generated.h"

/**
 * 
 */
UCLASS()
class GRD_API UGRDComboActionData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UGRDComboActionData();

	UPROPERTY(EditAnywhere, Category = "Name")
	FString MontageSectionNamePrefix;

	UPROPERTY(EditAnywhere, Category = "Name")
	uint8 MaxComboCount;

	UPROPERTY(EditAnywhere, Category = "Name")
	float FrameRate;

	UPROPERTY(EditAnywhere, Category = "ComboData")
	TArray<float> EffectiveFrameCount;
};