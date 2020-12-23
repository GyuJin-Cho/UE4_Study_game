// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (HUdOverlayAsset)
	{
		HudOverlay = CreateWidget<UUserWidget>(this, HUdOverlayAsset);
	}
	HudOverlay->AddToViewport();
	HudOverlay->SetVisibility(ESlateVisibility::Visible);
}