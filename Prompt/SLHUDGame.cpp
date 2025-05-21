#include "HUD/SLHUDGame.h"

#include "CustomPlayer.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "HUD/PromptHUDWidget.h"
#include "HUD/SLPageBase.h"
#include "Kismet/GameplayStatics.h"

void ASLHUDGame::BeginPlay()
{
	Super::BeginPlay();

	// ------------------- Prompt ------------------- //
	ACustomPlayer* Player = Cast<ACustomPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), ACustomPlayer::StaticClass()));
	if (IsValid(Player))
	{
		// Called when the player is looking at a movable object in range
		Player->OnActivateGrabPrompt.AddDynamic(this, &ASLHUDGame::ActivateGrabPrompt);
		// Called when the player collides with the collision sphere of a movable object
		Player->OnCountNearMovableObjects.AddDynamic(this, &ASLHUDGame::CountNearMovableObjects);
		// Called when the player grabs a movable object
		Player->OnObjectGrabbed.AddDynamic(this, &ASLHUDGame::ActivateReleasePrompt);
		// Called when the player attaches an object to a stand
		Player->OnObjectAttachedToStand.AddDynamic(this, &ASLHUDGame::ActivateEnterRotationPrompt);
		// Called when the player activates rotation mode
		Player->OnRotationModeActivated.AddDynamic(this, &ASLHUDGame::ActivateRotationModePrompt);
		// Called when the player is looking at the door and he is in range
		Player->OnDoorNear.AddDynamic(this, &ASLHUDGame::ActivateDoorPrompt);
	}

	CreatePromptHUDWidget();
}


// ------------------- Prompt ------------------- //

void ASLHUDGame::CreatePromptHUDWidget()
{
	ensureMsgf(PromptHUDWidgetClass, TEXT("Invalid prompt widget class in SLHUD"));
	if (!PromptHUDWidgetClass) { return; }

	PromptHUDWidget = Cast<UPromptHUDWidget>(UWidgetBlueprintLibrary::Create(this,
						PromptHUDWidgetClass, nullptr));
}


void ASLHUDGame::CountNearMovableObjects(const bool InIsMovableObjectNear)
{
	if (!IsValid(PromptHUDWidget)) { return; }

	if (InIsMovableObjectNear)
	{
		NearMovableObjectCount++;
	}else
	{
		NearMovableObjectCount--;
		NearMovableObjectCount = FMath::Clamp(NearMovableObjectCount, 0, 50);
		
		if (NearMovableObjectCount == 0)
		{
			ActivateGrabPrompt(false);
		}
	}
}

void ASLHUDGame::ActivateGrabPrompt(const bool InIsPlayerLookingAtObject)
{
	if (!PromptHUDWidget) { return; }

	bool IsPromptWidgetInViewport = PromptHUDWidget->IsVisible();
	
	if (!IsPromptWidgetInViewport && InIsPlayerLookingAtObject)
	{
		PromptHUDWidget->AddToViewport(5);
		PromptHUDWidget->SetGrabPromptWidgetActive(true);
		
	}else if (IsPromptWidgetInViewport && !InIsPlayerLookingAtObject)
	{
		PromptHUDWidget->SetGrabPromptWidgetActive(false);
		PromptHUDWidget->RemoveFromParent();
	}
}

void ASLHUDGame::ActivateReleasePrompt(const bool InIsObjectGrabbed)
{
	if (!PromptHUDWidget->IsVisible() && InIsObjectGrabbed)
	{
		PromptHUDWidget->AddToViewport(5);
	}
	PromptHUDWidget->SwitchGrabReleasePrompt(InIsObjectGrabbed);
}

void ASLHUDGame::ActivateEnterRotationPrompt(const bool InIsObjectAttachedToStand)
{
	PromptHUDWidget->SetEnterRotationPrompActive(InIsObjectAttachedToStand);
}

void ASLHUDGame::ActivateRotationModePrompt(const bool InIsObjectAttachedToStand)
{
	PromptHUDWidget->SetRotationModePromptActive(InIsObjectAttachedToStand);
}

void ASLHUDGame::ActivateDoorPrompt(const bool InIsDoorNear)
{
	if (!PromptHUDWidget) { return; }
	
	bool IsPromptWidgetInViewport = PromptHUDWidget->IsVisible();
	
	if (!IsPromptWidgetInViewport && InIsDoorNear)
	{
		PromptHUDWidget->AddToViewport(5);
		PromptHUDWidget->SetDoorPromptActive(InIsDoorNear);
		
	}else if (IsPromptWidgetInViewport && !InIsDoorNear)
	{
		PromptHUDWidget->SetDoorPromptActive(InIsDoorNear);
		PromptHUDWidget->RemoveFromParent();
	}
}


// --------------------- Page Menu In
void ASLHUDGame::AddPage(TSubclassOf<USLPageBase> InPageBase, FName InPageName)
{
	ensureMsgf(InPageBase, TEXT("Invalid Page class in SLHUD"));
	if (!InPageBase) {return;}

	USLPageBase* pageToAdd = Cast<USLPageBase>(UWidgetBlueprintLibrary::Create
		(this, InPageBase, GetOwningPlayerController()));

	GamePagesMap.Emplace(InPageName, pageToAdd);

	ensureMsgf(InPageBase, TEXT("Page is not in SLHUD"));
	if (!InPageBase) {return;}
	pageToAdd->AddToViewport(30);
}

void ASLHUDGame::RemovePage(FName InPageName)
{
	TObjectPtr<USLPageBase>* pageBasePtrToPtr = GamePagesMap.Find(InPageName);
	if (pageBasePtrToPtr)
	{
		TObjectPtr<USLPageBase> pageBasePtr = *pageBasePtrToPtr;
		pageBasePtr->RemoveFromParent();
	}
	GamePagesMap.Remove(InPageName);
}
// --------------------- Page Menu Out