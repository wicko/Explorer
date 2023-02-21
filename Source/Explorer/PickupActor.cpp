// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupActor.h"
#include "ExplorerCharacter.h"

// Sets default values
APickupActor::APickupActor()
{
	spriteComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Sprite"));
	spriteComponent->SetupAttachment(RootComponent);


	collisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	collisionSphere->SetupAttachment(spriteComponent);
	collisionSphere->SetSphereRadius(32.f);
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APickupActor::BeginPlay()
{
	Super::BeginPlay();
	collisionSphere->OnComponentBeginOverlap.AddDynamic(this, &APickupActor::OnSphereBeginOverlap);
	
}

// Called every frame
void APickupActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void APickupActor::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Checking if it is a First Person Character overlapping
	AExplorerCharacter* Character = Cast<AExplorerCharacter>(OtherActor);
	if (Character != nullptr)
	{
		// Notify that the actor is being picked up
		if (Character->InteractWithPickup(this))
		{
			Destroy();
		}
		else
		{
			return;
		}
	

		// Unregister from the Overlap Event so it is no longer triggered
		collisionSphere->OnComponentBeginOverlap.RemoveAll(this);
	}
}

