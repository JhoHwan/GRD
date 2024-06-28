// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_TurnToTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/GRDCharacterAIInterface.h"
#include "Kismet/KismetMathLibrary.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	NodeName = TEXT("Turn");
    bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        return EBTNodeResult::Failed;
    }

    APawn* AIPawn = AIController->GetPawn();
    if (!AIPawn)
    {
        return EBTNodeResult::Failed;
    }

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp)
    {
        return EBTNodeResult::Failed;
    }

    AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject("TargetToFollow"));
    if (!TargetActor)
    {
        return EBTNodeResult::Failed;
    }

    if (IsFacingPlayer(AIPawn, TargetActor, 20.0f))
    {
        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::InProgress;

}

void UBTTask_TurnToTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    APawn* AIPawn = AIController->GetPawn();
    if (!AIPawn)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject("TargetToFollow"));
    if (!TargetActor)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    if (IsFacingPlayer(AIPawn, TargetActor, 15.0f))
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        return;
    }

    FVector Direction = (TargetActor->GetActorLocation() - AIPawn->GetActorLocation()).GetSafeNormal();
    Direction.Z = 0;
    FRotator NewControlRotation = Direction.Rotation();
    FRotator CurrentRotation = AIPawn->GetActorRotation();
    FRotator InterpRotation = FMath::RInterpTo(CurrentRotation, NewControlRotation, DeltaSeconds, 5.0f);

    AIPawn->SetActorRotation(InterpRotation);
}

bool UBTTask_TurnToTarget::IsFacingPlayer(AActor* AIActor, AActor* TargetActor, float ToleranceDegrees) const
{
    if (!AIActor || !TargetActor)
    {
        return false;
    }

    FVector DirectionToPlayer = (TargetActor->GetActorLocation() - AIActor->GetActorLocation()).GetSafeNormal();
    FVector ForwardVector = AIActor->GetActorForwardVector();

    DirectionToPlayer.Z = 0;
    ForwardVector.Z = 0;

    float DotProduct = FVector::DotProduct(ForwardVector, DirectionToPlayer);
    float AngleDegrees = FMath::RadiansToDegrees(FMath::Acos(DotProduct));

    return AngleDegrees <= ToleranceDegrees;
}