// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "PaperFlipbookComponent.h"
#include "Components/SphereComponent.h"
#include "PickupActor.generated.h"


USTRUCT(BlueprintType)
struct FPickup
{
	GENERATED_USTRUCT_BODY()

	//what this pickup contains 
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer PickUpTags;

	// the amount the pickup contains
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 amount = 0;

	//the lifetime of the pickup (how long it will stay on the ground. 0 = infinite.)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 lifeTime = 0;

};

UCLASS()
class EXPLORER_API APickupActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbookComponent* sprite = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPickup pickUpData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UWeaponDataAsset* weaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* collisionSphere = nullptr;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UPaperFlipbookComponent* spriteComponent;

	/** Code for when something overlaps this component */
	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Returns Sprite subobject **/
	FORCEINLINE  UPaperFlipbookComponent* GetSprite() const { return spriteComponent; }

};
