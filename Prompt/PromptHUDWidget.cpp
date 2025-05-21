#include "HUD/PromptHUDWidget.h"

#include "Components/HorizontalBox.h"
#include "Components/VerticalBox.h"
#include "HUD/DoorPromptWidget.h"
#include "HUD/EnterRotationPrompt.h"
#include "HUD/GrabPromptWidget.h"
#include "HUD/RotationModePrompt.h"

void UPromptHUDWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	
	GrabPromptWidget = Cast<UGrabPromptWidget>(CreateWidget(this,
						GrabPromptWidgetClass, TEXT("GrabPrompt")));

	EnterRotationPrompt = Cast<UEnterRotationPrompt>(CreateWidget(this,
							EnterRotationPromptClass, TEXT("EnterRotationPrompt")));

	RotationModePrompt = Cast<URotationModePrompt>(CreateWidget(this,
							RotationModePromptClass, TEXT("RotationModePrompt")));

	DoorPromptWidget = Cast<UDoorPromptWidget>(CreateWidget(this,
							DoorPromptWidgetClass, TEXT("DoorPromptWidget")));
}

void UPromptHUDWidget::SetGrabPromptWidgetActive(const bool InSetActive)
{
	if (!GrabPromptWidget) { return; }

	bool IsGrabPromptVisible = GrabPromptWidget->IsVisible();

	if (InSetActive && !IsGrabPromptVisible)
	{
		PromptHorizontalBox->AddChildToHorizontalBox(GrabPromptWidget);
		GrabPromptWidget->SetSwitcherIndex(0);
	}
	else if (!InSetActive && IsGrabPromptVisible)
	{
		GrabPromptWidget->RemoveFromParent();
	}
}

void UPromptHUDWidget::SwitchGrabReleasePrompt(const bool InIsObjectGrabbed)
{
	if (!GrabPromptWidget) { return; }
	
	if (InIsObjectGrabbed)
	{
		if (!GrabPromptWidget->IsVisible())
		{
			PromptHorizontalBox->AddChildToHorizontalBox(GrabPromptWidget);
		}
		
		GrabPromptWidget->SetSwitcherIndex(1);
	}
	else
	{
		GrabPromptWidget->SetSwitcherIndex(0);
	}
}

void UPromptHUDWidget::SetEnterRotationPrompActive(const bool InSetActive)
{
	if (!EnterRotationPrompt) { return; }

	bool IsEnterRotationPromptVisible = EnterRotationPrompt->IsVisible();
	
	if (InSetActive && !IsEnterRotationPromptVisible)
	{
		PromptHorizontalBox->AddChildToHorizontalBox(EnterRotationPrompt);
		if (GrabPromptWidget->IsVisible())
			{EnterRotationPrompt->SetPadding(FMargin(HorizontalBoxLeftPadding, 0, 0, 0));}
		EnterRotationPrompt->SetSwitcherIndex(0);
	}
	else if (!InSetActive && IsEnterRotationPromptVisible)
	{
		EnterRotationPrompt->SetPadding(FMargin(0, 0, 0, 0));
		EnterRotationPrompt->RemoveFromParent();
	}
}

void UPromptHUDWidget::SwitchEnterRotationPrompt(const bool InIsInRotationMode)
{
	if (!EnterRotationPrompt) {return;}
	if (!EnterRotationPrompt->IsVisible()) {return;}

	if (InIsInRotationMode)
	{
		EnterRotationPrompt->SetSwitcherIndex(1);
		SetGrabPromptWidgetActive(false);
	}
	else
	{
		EnterRotationPrompt->SetSwitcherIndex(0);
	}
}

void UPromptHUDWidget::SetRotationModePromptActive(const bool InSetActive)
{
	if (!RotationModePrompt) { return; }

	bool IsRotationModePromptVisible = RotationModePrompt->IsVisible();

	if (InSetActive & !IsRotationModePromptVisible)
	{
		RotationModePrompt->AddToViewport(10);
		EnterRotationPrompt->SetSwitcherIndex(1);
		GrabPromptWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	else if (!InSetActive & IsRotationModePromptVisible)
	{
		RotationModePrompt->RemoveFromParent();
		EnterRotationPrompt->SetSwitcherIndex(0);
		GrabPromptWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void UPromptHUDWidget::SetDoorPromptActive(const bool InSetActive)
{
	if (!DoorPromptWidget) { return; }

	bool IsDoorPromptVisible = DoorPromptWidget->IsVisible();
	
	if (InSetActive && !IsDoorPromptVisible)
	{
		PromptHorizontalBox->AddChildToHorizontalBox(DoorPromptWidget);
	}
	else if (!InSetActive && IsDoorPromptVisible)
	{
		DoorPromptWidget->RemoveFromParent();
	}
}