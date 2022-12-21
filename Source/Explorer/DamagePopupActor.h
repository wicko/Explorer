// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/WidgetComponent.h"
#include "DamageInterface.h"
#include "DamagePopupActor.generated.h"

UCLASS()
class EXPLORER_API ADamagePopupActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADamagePopupActor();


	UPROPERTY()
	UWidgetComponent* widgetComp = nullptr;

	UPROPERTY()
	FDamagePackage incDamage = FDamagePackage();

	UPROPERTY()
	FVector spawnLoc = FVector();

	UPROPERTY()
	FRotator spawnRotate = FRotator();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D drawSize = FVector2D(30, 30);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class APlayerCameraManager* sourceCamera = nullptr;

	UFUNCTION()
	void CreateDamagePopup(FDamagePackage damagePack, FVector target, FRotator rotation);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	float lifetime = 1.5f;
	float currentLife = 0.0f;
	TSubclassOf<class UDisplayTextWidget> DamagePopUpClass;
	class UDisplayTextWidget* popUpWidget = nullptr;
	class APlayerCameraManager* camera = nullptr;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
