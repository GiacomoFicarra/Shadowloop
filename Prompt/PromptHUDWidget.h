#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PromptHUDWidget.generated.h"

class UHorizontalBox;
class UDoorPromptWidget;
class URotationModePrompt;
class UEnterRotationPrompt;
class UGrabPromptWidget;

UCLASS()
class SHADOWLOOP_API UPromptHUDWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;
	
protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UHorizontalBox> PromptHorizontalBox = nullptr;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGrabPromptWidget> GrabPromptWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UEnterRotationPrompt> EnterRotationPromptClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<URotationModePrompt> RotationModePromptClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UDoorPromptWidget> DoorPromptWidgetClass;

	UPROPERTY(EditAnywhere, Category="Padding")
	double HorizontalBoxLeftPadding = 100;

private:
	UPROPERTY()
	TObjectPtr<UGrabPromptWidget> GrabPromptWidget;

	UPROPERTY()
	TObjectPtr<UEnterRotationPrompt> EnterRotationPrompt;

	UPROPERTY()
	TObjectPtr<URotationModePrompt> RotationModePrompt;

	UPROPERTY()
	TObjectPtr<UDoorPromptWidget> DoorPromptWidget;

public:
	UFUNCTION()
	void SetGrabPromptWidgetActive(const bool InSetActive);

	UFUNCTION()
	void SwitchGrabReleasePrompt(const bool InIsObjectGrabbed);

	UFUNCTION()
	void SetEnterRotationPrompActive(const bool InSetActive);

	UFUNCTION()
	void SwitchEnterRotationPrompt(const bool InIsInRotationMode);

	UFUNCTION()
	void SetRotationModePromptActive(const bool InSetActive);

	UFUNCTION()
	void SetDoorPromptActive(const bool InSetActive);
};