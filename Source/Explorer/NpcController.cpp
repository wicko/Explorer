// Fill out your copyright notice in the Description page of Project Settings.


#include "NpcController.h"

ANpcController::ANpcController()
{
	BehaviorTree = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTree"));
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
}

void ANpcController::OnPossess(APawn* InPawn)
{

	Super::OnPossess(InPawn);

	npcAvatar = Cast<AExplorerPaperCharacter>(InPawn);
	if (npcAvatar)
	{
		if (npcAvatar->BehaviorTree->BlackboardAsset)
		{
			BlackboardComp->InitializeBlackboard(*npcAvatar->BehaviorTree->BlackboardAsset);

			//Make sure the Blackboard has the type of bot we possessed
			BlackboardComp->SetValueAsEnum(EnemyTypeKeyName, (uint8)npcAvatar->enemyType);

			SetInvestigateBehavior(npcAvatar->willMoveOnAlert);
		}

		BehaviorTree->StartTree(*npcAvatar->BehaviorTree);
	}
}

void ANpcController::OnUnPossess()
{
	Super::OnUnPossess();

	/* Stop any behavior running as we no longer have a pawn to control */
	BehaviorTree->StopTree();
}
void ANpcController::SetEnemyTarget(APawn* target)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsObject(TargetKeyName, target);
	}

}

void ANpcController::SetBlackBoardBehaveType(EStateAI behaveType)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsEnum(BehaviorTypeKeyName, (uint8)behaveType);
	}
}

//void ANpcController::SetBlackBoardPatrolPointType(APatrolPoint* patrolPoint)
//{
//	if (BlackboardComp)
//	{
//		BlackboardComp->SetValueAsObject(PatrolWaypointTypeKeyName, patrolPoint);
//	}
//}

void ANpcController::SetBlackBoardAlertLocationType(FVector alertLoc)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsVector(AlertLocationKeyName, alertLoc);
	}
}

void ANpcController::SetInvestigateBehavior(bool willInvestigate)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsBool(WillInvestigateKeyName, willInvestigate);
	}
}
void ANpcController::SetBlackBoardAction(UObject* SetActionSelected)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsObject(SelectedActionKeyName, SetActionSelected);

	}
}
void ANpcController::SetBlackBoardAvatarState(EAvatarState newState)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsEnum(SelectedAvatarStateKeyName, (uint8)newState);

	}
}
