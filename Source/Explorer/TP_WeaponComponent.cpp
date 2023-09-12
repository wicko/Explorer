// Copyright Epic Games, Inc. All Rights Reserved.


#include "TP_WeaponComponent.h"
#include "ExplorerCharacter.h"
#include "ExplorerProjectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "PaperFlipbook.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values for this component's properties
UTP_WeaponComponent::UTP_WeaponComponent()
{
	// Default offset from the character location for projectiles to spawn
	debugMuzzleOrigin = CreateDefaultSubobject<UStaticMeshComponent>(FName("MuzzleLocDebug"));
	debugMuzzleOrigin->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	debugMuzzleOrigin->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
	//debugMuzzleOrigin->SetRelativeLocation(MuzzleOffset);
}


void UTP_WeaponComponent::Fire()
{
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}

	// Try and fire a projectile
	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			CreateProjectile(ProjectileClass, false);
		}
	}
	
	// Try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}
	
	// Try and play a firing animation if specified
	if (firingAnim != nullptr)
	{
		this->SetFlipbook(firingAnim);
		this->PlayFromStart();
		this->SetLooping(false);
		// Get the animation object for the arms mesh
	//	UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
	//	if (AnimInstance != nullptr)
	//	{
	//		AnimInstance->Montage_Play(FireAnimation, 1.f);
	//	}
	}
}

void UTP_WeaponComponent::HoldFireStart(const FInputActionValue& Value)
{

	float heldValue = Value.Get<float>();
	currentChargeTime = currentChargeTime + heldValue;
//	UE_LOG(LogTemp, Log, TEXT("Charge Hold %f"), currentChargeTime);
/*	if (currentChargeTime > chargeTime)
	{
		UE_LOG(LogTemp, Log, TEXT("Hold Fire has been Started"));
	}*/
}

void UTP_WeaponComponent::HoldFireRelease(const FInputActionValue& Value)
{
	if (currentChargeTime > chargeTime)
	{
	//	UE_LOG(LogTemp, Log, TEXT("Hold Fire has been Released"));
		if (AltProjectileClass != nullptr)
		{
			UWorld* const World = GetWorld();
			if (World != nullptr)
			{
				CreateProjectile(AltProjectileClass, true);
			}
		}
	}
	currentChargeTime = 0.0f;

}
void UTP_WeaponComponent::CancelHold(const FInputActionValue& Value)
{
	currentChargeTime = 0.0f;
	UE_LOG(LogTemp, Log, TEXT("Cancelled Hold"));
}

void UTP_WeaponComponent::AttachWeapon(AExplorerCharacter* TargetCharacter)
{
	Character = TargetCharacter;
	if (Character == nullptr)
	{
		return;
	}
	if (Character->bHasRifle)
	{
		return;
	}
	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->weaponAnchor, FAttachmentTransformRules::KeepRelativeTransform);
	
	// switch bHasRifle so the animation blueprint can switch to another animation set
	Character->SetHasRifle(true, this);

	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// Fire
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::Fire);
			EnhancedInputComponent->BindAction(HoldFireAction, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::HoldFireStart);
			EnhancedInputComponent->BindAction(HoldFireAction, ETriggerEvent::Completed, this, &UTP_WeaponComponent::HoldFireRelease);
			EnhancedInputComponent->BindAction(HoldFireAction, ETriggerEvent::Canceled, this, &UTP_WeaponComponent::CancelHold);
		}
	}
	
	debugMuzzleOrigin->SetRelativeLocation(MuzzleOffset);
}

void UTP_WeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Character == nullptr)
	{
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(FireMappingContext);
		}
	}
}
void UTP_WeaponComponent::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	if (idleAnim) {
		SetFlipbook(idleAnim);
	}
	this->OnFinishedPlaying.AddDynamic(this, &UTP_WeaponComponent::SetIdleAnim);


}
void UTP_WeaponComponent::CreateProjectile(TSubclassOf<AExplorerProjectile> projectileType, bool isAltFire)
{


	GetFlipbook()->FindSocket(FName("Muzzle"), 0, socketLoc);
	APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
	const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
	// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
	const FVector SpawnLocation = this->GetComponentLocation() + FTransform(SpawnRotation).TransformVector(socketLoc.GetTranslation()) + FTransform(SpawnRotation).TransformVector(MuzzleOffset);//PlayerController->PlayerCameraManager->GetCameraLocation() + FTransform(SpawnRotation).TransformVector(MuzzleOffset); //SpawnRotation.RotateVector(MuzzleOffset);

	//Set Spawn Collision Handling Override
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	ActorSpawnParams.Owner = Character;

	// Spawn the projectile at the muzzle
	FTransform projTrans;
	projTrans.SetRotation(SpawnRotation.Quaternion());
	projTrans.SetTranslation(SpawnLocation);

//	UE_LOG(LogTemp, Log, TEXT("Prjectile Spawn at %s, facing %s"), *SpawnLocation.ToString(), *SpawnRotation.ToString());
	FHitResult hitResult;
	TArray<FHitResult> hitArray;
	AExplorerProjectile* proj = GetWorld()->SpawnActorDeferred<AExplorerProjectile>(projectileType, FTransform(), Character);
	proj->Owner = Character;

	proj->dmgPack = isAltFire ? altDmgPack : dmgPack;
	switch (proj->projType)
	{
	case EProjectileType::HITSCAN:


	//	GetWorld()->LineTraceSingleByChannel(hitResult, SpawnLocation, projTrans.GetTranslation() + (projTrans.GetRotation().Rotator().Vector() * altFireDistance), ECollisionChannel::ECC_GameTraceChannel1);
		GetWorld()->LineTraceMultiByChannel(hitArray, SpawnLocation, projTrans.GetTranslation() + (projTrans.GetRotation().Rotator().Vector() * altFireDistance), ECollisionChannel::ECC_GameTraceChannel1);
		for (FHitResult hit : hitArray)
		{
			proj->endPoints.Add(hit.ImpactPoint);
		}
	//	proj->endPoint = hitResult.IsValidBlockingHit() ? hitResult.ImpactPoint : projTrans.GetTranslation() + (projTrans.GetRotation().Rotator().Vector() * altFireDistance);
	case EProjectileType::PROJECTILE:

	case EProjectileType::SPECIAL:

	default:
		break;
	}
	proj->spawnTrans = projTrans;
	proj->distance = altFireDistance;
	proj->dmgPack.damageSource = Character;
	UGameplayStatics::FinishSpawningActor(proj, projTrans);

}
void UTP_WeaponComponent::SetIdleAnim()
{
	this->SetLooping(true);
	if (idleAnim != nullptr)
	{
		this->SetFlipbook(idleAnim);
	}
}
