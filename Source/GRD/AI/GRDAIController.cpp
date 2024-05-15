// Fill out your copyright notice in the Description page of Project Settings.


#include "GRDAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Character/GRDCharacterPlayer.h"

AGRDAIController::AGRDAIController()
{

}

void AGRDAIController::RunAI(ACharacter* InPlayerCharacter)
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	if (UseBlackboard(BBAsset, BlackboardPtr))
	{	
		
		Blackboard->SetValueAsObject(TEXT("AttackTarget"), InPlayerCharacter);
		bool RunResult = RunBehaviorTree(BTAsset);
		ensure(RunResult);
	}
}

void AGRDAIController::StopAI()
{
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComponent)
	{
		BTComponent->StopTree();
	}
}

void AGRDAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}
