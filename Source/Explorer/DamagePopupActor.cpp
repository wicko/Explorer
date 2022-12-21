// Fill out your copyright notice in the Description page of Project Settings.


#include "DamagePopupActor.h"
#include "DisplayTextWidget.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
ADamagePopupActor::ADamagePopupActor()
{

	static ConstructorHelpers::FClassFinder<UDisplayTextWidget> BP_DmgPopUp(TEXT("/Game/UI/DamagePopUp_UMG"));

	widgetComp = CreateDefaultSubobject<UWidgetComponent>(FName("Damage Popup Widget"));
	//widgetComp->SetDrawSize(drawSize);
	//widgetComp->SetRenderCustomDepth(true);
	widgetComp->SetRenderInDepthPass(false);
	//widgetComp->SetDepthPriorityGroup(ESceneDepthPriorityGroup::SDPG_MAX);
	DamagePopUpClass = BP_DmgPopUp.Class;

	RootComponent = widgetComp;
	
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ADamagePopupActor::CreateDamagePopup(FDamagePackage damagePack, FVector target, FRotator rotation)
{
	popUpWidget = CreateWidget<UDisplayTextWidget>(GetWorld(), DamagePopUpClass);
	widgetComp->SetWidget(popUpWidget);
	this->SetActorLocation(target);
	this->SetActorRotation(rotation);
	this->widgetComp->SetCastShadow(false);
	popUpWidget->ApplyDamagePopup(damagePack);

}

// Called when the game starts or when spawned
void ADamagePopupActor::BeginPlay()
{
	Super::BeginPlay();
	CreateDamagePopup(incDamage, spawnLoc, spawnRotate);
}

// Called every frame
void ADamagePopupActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	currentLife = currentLife + DeltaTime;
	this->AddActorLocalOffset(FVector(0.0f, 0.0f, 0.15f));
	//widgetComp->SetTintColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f - (currentLife / lifetime)));
	if (sourceCamera)
	{
		this->SetActorRotation(FRotator(0.0f, sourceCamera->GetCameraRotation().Yaw + 180, 0.0f));
	}
	widgetComp->GetWidget()->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f - (currentLife/ lifetime)));
	if (currentLife > lifetime)
	{
		this->Destroy();
	}
}

