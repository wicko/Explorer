// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplorerController.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "Camera/CameraComponent.h"


AExplorerController::AExplorerController()
{


}

void AExplorerController::BeginPlay()
{
	Super::BeginPlay();

	appliedWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
	SwapWidget(appliedWidget);
}

void AExplorerController::CreateLoadScreen()
{

	appliedWidget = CreateWidget<UUserWidget>(GetWorld(), LoadScreenWidgetClass);
	SwapWidget(appliedWidget);
}

void AExplorerController::SwapWidget(UUserWidget* widget)
{
	if (widget == nullptr)
	{
		return;
	}

	if (currentWidget == widget)
	{
		return;
	}

	if (IsValid(currentWidget))
	{
		//currentWidget->RemoveFromViewport();
		currentWidget->RemoveFromParent();
		currentWidget = nullptr;
	}

	widget->AddToViewport();
	currentWidget = widget;

}

