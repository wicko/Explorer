// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplorerProjectile_Beam.h"
#include "Kismet/GameplayStatics.h"

AExplorerProjectile_Beam::AExplorerProjectile_Beam()
{

}

void AExplorerProjectile_Beam::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{


}
void AExplorerProjectile_Beam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called when the game starts or when spawned
void AExplorerProjectile_Beam::BeginPlay()
{
	Super::BeginPlay();

/*	FCollisionObjectQueryParams CollisionObjectParams;
	for (auto Iter = HitObjectTypes.CreateConstIterator(); Iter; ++Iter)
	{
		const ECollisionChannel& Channel = UCollisionProfile::Get()->ConvertToCollisionChannel(false, *Iter);
		CollisionObjectParams.AddObjectTypesToQuery(Channel);
	}*/

	FCollisionShape sweepShape = FCollisionShape::MakeBox(FVector(5.0f, 5.0f, 5.0f));
	FCollisionQueryParams CollisionQueryParams;
	FCollisionObjectQueryParams ObjectQueryParams;
	CollisionQueryParams.AddIgnoredActor(dmgPack.damageSource);
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel2);

	TArray<AActor*> actorsToIgnore;
	actorsToIgnore.Add(dmgPack.damageSource);

	TArray<FHitResult> hitArray;
	FVector targetLoc = spawnTrans.GetTranslation() + (spawnTrans.GetRotation().Rotator().Vector() * distance);
	const bool doesHit = GetWorld()->SweepMultiByChannel(hitArray, spawnTrans.GetTranslation(), targetLoc, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel2, sweepShape, CollisionQueryParams);
	//const bool doesHit = UKismetSystemLibrary::LineTraceMultiForObjects(GetWorld(), spawnTrans.GetTranslation(), spawnTrans.GetTranslation() + (spawnTrans.GetRotation().Rotator().Vector() * distance), HitObjectTypes, true, actorsToIgnore, EDrawDebugTrace::ForDuration, hitArray, true);
	UNiagaraComponent* Beam = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BeamEffect, spawnTrans.GetLocation(), spawnTrans.GetRotation().Rotator());
	Beam->SetNiagaraVariablePosition(FString("BeamEndPoint"), endPoints.IsEmpty() ? targetLoc : endPoints.Last());

	//add hit components to array and check so you don't hit the same limb twice
	TArray<UPrimitiveComponent*> hitTargets;

	for (FHitResult hit : hitArray)
	{
		AActor* hitActor = hit.GetActor();
		if (hitActor != nullptr)
		{
			if (Cast<IDamageInterface>(hitActor))
			{
				if (hit.GetComponent() != nullptr && !hitTargets.Contains(hit.GetComponent()))
				{
					Cast<IDamageInterface>(hitActor)->Execute_ReactToDamage(hitActor, dmgPack, hit.GetComponent());
					if (hit.GetActor() != nullptr)
					{
						GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("The target Being Hit is: %s"), *hit.GetActor()->GetName()));
					}
					else if (hit.GetComponent() != nullptr)
					{
						GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("The component Being Hit is: %s"), *hit.GetComponent()->GetName()));
					}
					hitTargets.Add(hit.GetComponent());
				}
				
			}
		
		}
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), contactEffect, hit.ImpactPoint, hit.ImpactNormal.Rotation());
		DrawDebugSphere(GetWorld(), hit.ImpactPoint, 20.0f, 10, FColor::Green, false, 5.0f);
	}


	

}