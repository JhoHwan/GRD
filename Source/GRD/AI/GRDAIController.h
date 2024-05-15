// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GRDAIController.generated.h"

/**
 * 
 */
UCLASS()
class GRD_API AGRDAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AGRDAIController();

	UFUNCTION(BlueprintCallable)
	void RunAI();
	void StopAI();

protected:
	virtual void OnPossess(APawn* InPawn) override;

	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class UBlackboardData> BBAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class UBehaviorTree> BTAsset;
};
