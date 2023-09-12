// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplorerProjectile_Bullet.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

AExplorerProjectile_Bullet::AExplorerProjectile_Bullet()
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");

	CollisionComp->OnComponentHit.AddDynamic(this, &AExplorerProjectile_Bullet::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	projectileSprite = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Sprite"));
	projectileSprite->SetupAttachment(RootComponent);
	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = canBounce;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	PrimaryActorTick.bCanEverTick = true;
}

void AExplorerProjectile_Bullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
	if ((OtherActor == nullptr) || (OtherActor == this))
	{
		if (!canBounce)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), contactEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
			Destroy();
		}
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("Projectile has hit %s"), *OtherActor->GetName());
	if (Cast<IDamageInterface>(OtherActor))
	{
		UE_LOG(LogTemp, Log, TEXT("Projectile has hit %s"), *OtherComp->GetName());
		dmgPack.hitVector = Hit.ImpactPoint;
		dmgPack.damage = 5;
		Cast<IDamageInterface>(OtherActor)->Execute_ReactToDamage(OtherActor, dmgPack, OtherComp);
	}
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), contactEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
	if (canBounce)
	{
		return;
	}
	else
	{
		Destroy();
	}
	// Only add impulse and destroy projectile if we hit a physics
	//if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	//{
	//	OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

	//	Destroy();
	//}
}
// Called every frame

void AExplorerProjectile_Bullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (playerCamera)
	{
		FRotator spriteRotation = FRotator(playerCamera->GetCameraRotation().Roll, playerCamera->GetCameraRotation().Yaw + 90.0f, playerCamera->GetCameraRotation().Pitch);
		projectileSprite->SetWorldRotation(spriteRotation);
	}
}

// Called when the game starts or when spawned
void AExplorerProjectile_Bullet::BeginPlay()
{
	Super::BeginPlay();
	CollisionComp->IgnoreActorWhenMoving(GetOwner(), true);
	APlayerController* controller = GetWorld()->GetFirstPlayerController();
	playerCamera = controller->PlayerCameraManager;

}