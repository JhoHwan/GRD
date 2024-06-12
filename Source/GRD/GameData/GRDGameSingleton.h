// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GRDGameSingleton.generated.h"

/**
 * 
 */
UCLASS()
class GRD_API UGRDGameSingleton : public UObject
{
	GENERATED_BODY()
	
public:
	UGRDGameSingleton();
	static UGRDGameSingleton& Get();

private:
	//TArray<> CharacterStatTable
};
