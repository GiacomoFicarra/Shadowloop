#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SLHUDGame.generated.h"

class USLPageBase;
class UPromptHUDWidget;

UCLASS()
class SHADOWLOOP_API ASLHUDGame : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

// ------------------- Prompt ------------------- //	
protected:	
	UFUNCTION()
	void ActivateGrabPrompt(const bool InIsPlayerLookingAtObject);

	UFUNCTION()
	void CountNearMovableObjects(const bool InIsMovableObjectNear);

	UFUNCTION()
	void ActivateReleasePrompt(const bool InIsObjectGrabbed);

	UFUNCTION()
	void ActivateEnterRotationPrompt(const bool InIsObjectAttachedToStand);

	UFUNCTION()
	void ActivateRotationModePrompt(const bool InIsObjectAttachedToStand);

	UFUNCTION()
	void ActivateDoorPrompt(const bool InIsDoorNear);
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UPromptHUDWidget> PromptHUDWidgetClass;

private:
	void CreatePromptHUDWidget();
	
	UPROPERTY()
	TObjectPtr<UPromptHUDWidget> PromptHUDWidget;
	
	int NearMovableObjectCount = 0;

// --------------------- Page HUD In
	TMap<FName, TObjectPtr<USLPageBase>> GamePagesMap;

public:
	UFUNCTION(BlueprintCallable, Category = "HUDGame | Meenu Page")
	void AddPage(TSubclassOf<USLPageBase> InPageBase, FName InPageName);

	UFUNCTION(BlueprintCallable, Category = "HUDGame | Meenu Page")
	void RemovePage(FName InPageName);
	
// --------------------- Page HUD End
};