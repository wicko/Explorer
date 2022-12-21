// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ExplorerPaperCharacter.h"
#include "NpcController.generated.h"

/**
 * 
 */
UCLASS()
class EXPLORER_API ANpcController : public AAIController
{
	GENERATED_BODY()
		ANpcController();

	UPROPERTY(EditAnywhere, Category = "NPC")
		AExplorerPaperCharacter* npcAvatar;

	UPROPERTY(EditAnywhere, Category = "AI", meta = (AllowPrivateAccess = "true"))
		UBlackboardComponent* BlackboardComp;

	UPROPERTY(EditAnywhere, Category = "AI", meta = (AllowPrivateAccess = "true"))
		UBehaviorTreeComponent* BehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		FName EnemyTypeKeyName = "TargetEntity";

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		FName FactionTypeKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		FName BehaviorTypeKeyName = "EnemyBehavior";

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		FName TargetKeyName = "TargetEntity";

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		FName AlertLocationKeyName = "AlertLocation";

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		FName WillInvestigateKeyName = "willMoveToInvestigate";

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		FName PatrolWaypointTypeKeyName = "CurrentPatrolPoint";

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		FName SelectedActionKeyName = "SelectedAction";

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		FName SelectedAvatarStateKeyName = "EnemyState";

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		FName CurrentWaypointTypeKeyName;

	virtual void OnPossess(class APawn* InPawn) override;
	virtual void OnUnPossess() override;

public:
	//sets the target that the enemy will be focusing on
	void SetEnemyTarget(APawn* target);

	//set whether the enemy will move on investigate or stay where they are
	void SetInvestigateBehavior(bool willInvestigate);

	//set the enemy's current behavior (patrol, idle, combat, etc)
	void SetBlackBoardBehaveType(EStateAI behaveType);

	//Set the current action being selected to use
	void SetBlackBoardAction(UObject* SetActionSelected);

	//set the enemy's current patrol waypoint
	//void SetBlackBoardPatrolPointType(APatrolPoint* patrolPoint);

	//set the enemy's current alert location, caused by sound or sight
	void SetBlackBoardAlertLocationType(FVector alertLoc);

	//set the enemy's current state (idle, action, falling, etc)
	void SetBlackBoardAvatarState(EAvatarState newState);

	/** Returns BehaviorComp subobject **/
	FORCEINLINE UBehaviorTreeComponent* GetBehaviorComp() const { return BehaviorTree; }

	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const { return BlackboardComp; }
};
