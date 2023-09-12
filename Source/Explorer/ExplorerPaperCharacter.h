// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "PaperCharacterLimb.h"
#include "DamageInterface.h"
#include "Components/TextRenderComponent.h"
#include "Components/AudioComponent.h"
#include "AIModule/Classes/Perception/PawnSensingComponent.h"
#include "AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "AIModule/Classes/BehaviorTree/BehaviorTree.h"
#include "ExplorerPaperCharacter.generated.h"



UENUM(BlueprintType)
enum class EStateAI : uint8
{
	AI_DEBUG_FREEZE		 UMETA(DisplayName = "Unresponsive (Debug)"),
	AI_IDLE				 UMETA(DisplayName = "Idle"),
	AI_INVESTIGATE		 UMETA(DisplayName = "Investigate"),
	AI_COMBAT			 UMETA(DisplayName = "Combat"),
	AI_DEAD				 UMETA(DisplayName = "Dead"),

};

enum class EAvatarState : uint8
{
	AS_NONE				 UMETA(DisplayName = "None"),

};

UENUM(BlueprintType)
enum class EEnemyMobility : uint8
{
	EM_GROUND		UMETA(DisplayName = "Ground Movement"),
	EM_FLYING		UMETA(DisplayName = "Flying Movement"),
	EM_IMMOBILE		UMETA(DisplayName = "Immobile"),

};

UENUM(BlueprintType)
enum class EEnemyTypes : uint8
{
	ET_DRONE		UMETA(DisplayName = "Drone"),

};
/**
 * 
 */
UCLASS()
class EXPLORER_API AExplorerPaperCharacter : public APaperCharacter, public IDamageInterface
{
	GENERATED_BODY()

	AExplorerPaperCharacter();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Sprite")
		TArray<UPaperCharacterLimb*> spriteLimbs = {};

	void UpdateAnimation();

	UPROPERTY(EditAnywhere, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UBlackboardComponent* BlackBoardComp;

	UPROPERTY(EditAnywhere, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UPawnSensingComponent* PlayerSensingComp;

	/** Whether or not an enemy is alerted to a location or target**/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	bool hasSensedTarget = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	bool isInCombat = false;

	/** Whether or not an enemy will move if they are alerted, or if they will stay until combat**/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = "true"))
	bool willMoveOnAlert = true;

	/** the time it takes for an enemy to register the target after seeing them (need a slight delay so they don't instantly aggro). The time between first spotting the target and this value, they will investigate (barring distance) */
	UPROPERTY(EditAnywhere, Category = AI, meta = (AllowPrivateAccess = "true"))
	float aggroTimer = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	EStateAI currentState = EStateAI::AI_IDLE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	EEnemyTypes enemyType = EEnemyTypes::ET_DRONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	int32 baseHealth = 40;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	int32 currentHealth = 40;

	//the rotation of the sprite rendering towards the player (separate from the capsule rotation which will control movement and designate the character's actual rotation)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprite")
	FRotator spriteRotation;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Damage Interface")
	void ReactToDamage(FDamagePackage damagePackage, USceneComponent* limb);
	virtual void ReactToDamage_Implementation(FDamagePackage damagePackage, USceneComponent* limb) override;

	/** AI Behavior: Enemy hears noise */
	UFUNCTION(BlueprintCallable)
		void BehaviorOnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume);

	/** AI Behavior: Enemy sees player */
	UFUNCTION(BlueprintCallable)
		void BehaviorOnSeePlayer(APawn* Pawn);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = AI, meta = (AllowPrivateAccess = "true"))
	UTextRenderComponent* debugTextComponent = nullptr;

	//** Perception **//
	UPROPERTY(BlueprintReadOnly, Category = "AI")
	float lastHeardTime = 0.0f;
	UPROPERTY(BlueprintReadOnly, Category = "AI")
	float lastSeenTime = 0.0f;

	/** the current aggro timer. Once this reaches aggroTimer value, the enemy has officially entered combat**/
	UPROPERTY(BlueprintReadOnly, Category = "AI")
	float currentAggroTimer = 0.0f;

	/** The distance that the enemy will instantly aggro onto you on sight (otherwise, they will investigate**/
	UPROPERTY(EditAnywhere, Category = AI, meta = (AllowPrivateAccess = "true"))
	float aggroDistance = 100.0f;

	/** the time after losing the players location where the enemy resets */
	UPROPERTY(EditAnywhere, Category = AI, meta = (AllowPrivateAccess = "true"))
	float ResetAggro = 5.0f;

	//the last location the NPC has detected the player (noise/sight)
	UPROPERTY(BlueprintReadOnly, Category = "AI")
	FVector alertLocation;
	//The NPC's current target 
	UPROPERTY(BlueprintReadOnly, Category = "AI")
	APawn* target = nullptr;


	//**  **//

	//** Combat **//
	UFUNCTION(BlueprintCallable)
		void OnDamageHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
		void OnDamageEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/** AI Behavior: Changes the debug text over the AI  */
	UFUNCTION(BlueprintCallable)
	void ChangeAIStateText(FText newText);

private:

	APlayerCameraManager* playerCamera = nullptr;

	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
