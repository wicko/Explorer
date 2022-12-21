// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplorerPaperCharacter.h"
#include "DamagePopupActor.h"
#include "ExplorerCharacter.h"
#include "NpcController.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

AExplorerPaperCharacter::AExplorerPaperCharacter()
{

	UPaperCharacterLimb* torsoLimb = CreateDefaultSubobject<UPaperCharacterLimb>(FName("Torso"));
	torsoLimb->AttachToComponent(GetSprite(), FAttachmentTransformRules::KeepWorldTransform);
	spriteLimbs.Add(torsoLimb);

	UPaperCharacterLimb* headLimb = CreateDefaultSubobject<UPaperCharacterLimb>(FName("Head"));
	headLimb->AttachToComponent(GetSprite(), FAttachmentTransformRules::KeepWorldTransform);
	spriteLimbs.Add(headLimb);

	UPaperCharacterLimb* rLegLimb = CreateDefaultSubobject<UPaperCharacterLimb>(FName("RightLeg"));
	rLegLimb->AttachToComponent(GetSprite(), FAttachmentTransformRules::KeepWorldTransform);
	spriteLimbs.Add(rLegLimb);

	UPaperCharacterLimb* lLegLimb = CreateDefaultSubobject<UPaperCharacterLimb>(FName("LeftLeg"));
	lLegLimb->AttachToComponent(GetSprite(), FAttachmentTransformRules::KeepWorldTransform);
	spriteLimbs.Add(lLegLimb);

	UPaperCharacterLimb* lArmLimb = CreateDefaultSubobject<UPaperCharacterLimb>(FName("LeftArm"));
	lArmLimb->AttachToComponent(GetSprite(), FAttachmentTransformRules::KeepWorldTransform);
	spriteLimbs.Add(lArmLimb);

	UPaperCharacterLimb* rArmLimb = CreateDefaultSubobject<UPaperCharacterLimb>(FName("RightArm"));
	rArmLimb->AttachToComponent(GetSprite(), FAttachmentTransformRules::KeepWorldTransform);
	spriteLimbs.Add(rArmLimb);

	debugTextComponent = CreateDefaultSubobject<UTextRenderComponent>("DebugText");
	debugTextComponent->AttachToComponent(GetSprite(), FAttachmentTransformRules::KeepWorldTransform);

	PlayerSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");
	PlayerSensingComp->SetPeripheralVisionAngle(60.0f);
	PlayerSensingComp->SightRadius = 1500;
	PlayerSensingComp->HearingThreshold = 600;
	PlayerSensingComp->LOSHearingThreshold = 1200;

	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AExplorerPaperCharacter::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* controller = GetWorld()->GetFirstPlayerController();
	playerCamera = controller->PlayerCameraManager;

	if (PlayerSensingComp)
	{
		PlayerSensingComp->OnSeePawn.AddDynamic(this, &AExplorerPaperCharacter::BehaviorOnSeePlayer);
		PlayerSensingComp->OnHearNoise.AddDynamic(this, &AExplorerPaperCharacter::BehaviorOnHearNoise);
	}
	ChangeAIStateText(FText());
}

void AExplorerPaperCharacter::ChangeAIStateText(FText newText)
{
	FString text = FString("Health: ");
	text.AppendInt(currentHealth);
	debugTextComponent->SetText(FText().FromString(text));
};

void AExplorerPaperCharacter::OnDamageHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this)
	{
		return;
	}
	//if the object hit doesn't contain the DamageInterface, continue on 

}

void AExplorerPaperCharacter::ReactToDamage_Implementation(FDamagePackage damagePackage, USceneComponent* limb)
{
	UPaperCharacterLimb* hitLimb = Cast<UPaperCharacterLimb>(limb);
	if (!hitLimb->limbData.isLimbRemoved)
	{
		hitLimb->limbData.limbHealth = hitLimb->limbData.limbHealth - damagePackage.damage;
		if (hitLimb->limbData.limbHealth <= 0 && hitLimb->limbData.limbName != ELimbName::LIMB_TORSO)
		{
			hitLimb->DestroyComponent();
		}
	}
	currentHealth = currentHealth - damagePackage.damage;

	//create damage pop up object and run it
	ADamagePopupActor* damagePopup = GetWorld()->SpawnActorDeferred<ADamagePopupActor>(ADamagePopupActor::StaticClass(), FTransform(), GetWorld()->GetFirstPlayerController());
	damagePopup->incDamage = damagePackage;
	damagePopup->spawnLoc = damagePackage.hitVector;
	damagePopup->spawnRotate = FRotator(0.0f, playerCamera->GetCameraRotation().Yaw + 180, 0.0f);
	damagePopup->sourceCamera = playerCamera;
	UGameplayStatics::FinishSpawningActor(damagePopup, FTransform());
	ChangeAIStateText(FText());
	if (currentHealth <= 0)
	{
		Destroy();
	}
}

void AExplorerPaperCharacter::OnDamageEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}


// Called every frame
void AExplorerPaperCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (playerCamera)
	{
		spriteRotation = FRotator(0.0f, playerCamera->GetCameraRotation().Yaw + 90, 0.0f);
		GetSprite()->SetWorldRotation(spriteRotation);
	}
	if (hasSensedTarget)
	{
		if (target) {
			if (!isInCombat)
			{
				if (!PlayerSensingComp->CouldSeePawn(target))
				{
					currentAggroTimer = 0;
				}
				else
				{
					currentAggroTimer += DeltaTime;
				}
			}
			if ((GetWorld()->TimeSeconds - lastSeenTime) > ResetAggro
				&& (GetWorld()->TimeSeconds - lastHeardTime) > ResetAggro) {
				ANpcController* controller = this->GetController<ANpcController>();
				if (controller)
				{
					hasSensedTarget = false;
					isInCombat = false;
					/* Reset */
					controller->SetEnemyTarget(nullptr);
					controller->SetBlackBoardBehaveType(EStateAI::AI_IDLE);
					/* Stop playing the hunting sound */
					//BroadcastUpdateAudioLoop(false);
				}
			}
			/*
			if (isSprinting && (target->GetActorLocation() - this->GetActorLocation()).Size() < switchToWalkDistance)
			{
				isSprinting = false;
				this->GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
			}
			else if (!isSprinting && (target->GetActorLocation() - this->GetActorLocation()).Size() >= switchToWalkDistance)
			{
				isSprinting = true;
				this->GetCharacterMovement()->MaxWalkSpeed = sprintSpeed;
			}
			*/
		}
	}


}

// Called to bind functionality to input
void AExplorerPaperCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AExplorerPaperCharacter::BehaviorOnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume)
{
	hasSensedTarget = true;
	lastHeardTime = GetWorld()->GetTimeSeconds();
	alertLocation = Location;
	ANpcController* controller = this->GetController<ANpcController>();
	if (controller)
	{
		controller->SetBlackBoardAlertLocationType(Location);
	}
}

void AExplorerPaperCharacter::BehaviorOnSeePlayer(APawn* Pawn)
{
	hasSensedTarget = true;
	lastSeenTime = GetWorld()->GetTimeSeconds();
	//if the enemy already has the target as their focus, return
	if (isInCombat)
	{
		return;
	}

	//if the enemy is within their instant aggro range, they will target them. Otherise, countdown the aggro timer since last seeing them

	if (Cast<AExplorerCharacter>(Pawn))
	{
		target = Cast<AExplorerCharacter>(Pawn);
	}

	if (currentAggroTimer >= aggroTimer || (Pawn->GetActorLocation() - this->GetActorLocation()).Size() < aggroDistance)
	{
		ANpcController* controller = this->GetController<ANpcController>();
		if (controller)
		{
			UE_LOG(LogTemp, Log, TEXT("Enemy has entered combat"));
			isInCombat = true;
			controller->SetEnemyTarget(target);
			controller->SetBlackBoardBehaveType(EStateAI::AI_COMBAT);
		}
	}
}

void AExplorerPaperCharacter::UpdateAnimation()
{

}