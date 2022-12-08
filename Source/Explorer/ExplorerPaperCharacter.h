// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "PaperCharacterLimb.h"
#include "DamageInterface.h"
#include "AIModule/Classes/Perception/PawnSensingComponent.h"
#include "AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "AIModule/Classes/BehaviorTree/BehaviorTree.h"
#include "ExplorerPaperCharacter.generated.h"

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

	UPROPERTY(EditAnywhere, Category = AI, meta = (AllowPrivateAccess = "true"))
	UPawnSensingComponent* PlayerSensingComp;

	/** Whether or not an enemy is alerted to a location or target**/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = "true"))
	bool hasSensedTarget = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	int32 baseHealth = 40;


	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Damage Interface")
	void ReactToDamage(FDamagePackage damagePackage, USceneComponent* limb);
	virtual void ReactToDamage_Implementation(FDamagePackage damagePackage, USceneComponent* limb) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//** Perception **//
	UPROPERTY(BlueprintReadOnly, Category = "AI")
	float lastHeardTime = 0.0f;
	UPROPERTY(BlueprintReadOnly, Category = "AI")
	float lastSeenTime = 0.0f;

	/** the current aggro timer. Once this reaches aggroTimer value, the enemy has officially entered combat**/
	UPROPERTY(BlueprintReadOnly, Category = "AI")
	float currentAggroTimer = 0.0f;
	//**  **//

	//** Combat **//
	UFUNCTION(BlueprintCallable)
		void OnDamageHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
		void OnDamageEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


private:

	APlayerCameraManager* playerCamera = nullptr;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
