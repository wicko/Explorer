// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplorerPaperCharacter.h"
#include "DamagePopupActor.h"
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
}

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
	baseHealth = baseHealth - damagePackage.damage;

	//create damage pop up object and run it
	ADamagePopupActor* damagePopup = GetWorld()->SpawnActorDeferred<ADamagePopupActor>(ADamagePopupActor::StaticClass(), FTransform(), GetWorld()->GetFirstPlayerController());
	damagePopup->incDamage = damagePackage;
	damagePopup->spawnLoc = damagePackage.hitVector;
	damagePopup->spawnRotate = FRotator(0.0f, playerCamera->GetCameraRotation().Yaw + 180, 0.0f);
	UGameplayStatics::FinishSpawningActor(damagePopup, FTransform());

	if (baseHealth <= 0)
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
		GetSprite()->SetWorldRotation(FRotator(0.0f, playerCamera->GetCameraRotation().Yaw + 90, 0.0f));
	}
}

// Called to bind functionality to input
void AExplorerPaperCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AExplorerPaperCharacter::UpdateAnimation()
{

}