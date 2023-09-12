// Copyright Epic Games, Inc. All Rights Reserved.

#include "ExplorerProjectile.h"


AExplorerProjectile::AExplorerProjectile() 
{


	




	// Die after 3 seconds by default
	InitialLifeSpan = 5.0f;
}

void AExplorerProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	if ((OtherActor == nullptr) || (OtherActor == this))
	{
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

void AExplorerProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called when the game starts or when spawned
void AExplorerProjectile::BeginPlay()
{
	Super::BeginPlay();


}