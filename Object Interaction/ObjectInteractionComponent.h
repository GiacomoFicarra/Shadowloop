#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "ShadowLoop/ShadowLoopCharacter.h"
#include "ObjectInteractionComponent.generated.h"

class ACustomPlayer;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHit, AActor*,HitResultActor);

UCLASS(ClassGroup=(CustomPlayerComponents), meta=(BlueprintSpawnableComponent))
class SHADOWLOOP_API UObjectInteractionComponent : public UPhysicsHandleComponent
{
	GENERATED_BODY()

	// ------------------- Grab ------------------- //
public:
	UFUNCTION(BlueprintCallable, Category="ObjectInteraction")
	virtual void GrabObject(UPrimitiveComponent* ComponentToGrab, AActor* InActor, const FName InBoneName);

	UFUNCTION(BlueprintCallable, Category="ObjectInteraction")
	virtual bool GrabObjectWithLineTrace();

	bool GetIsGrabbingObject() const { return bIsGrabbingObject; }
	
protected:
	UFUNCTION(BlueprintCallable, Category="ObjectInteraction")
	virtual void ReleaseObject();
	
	UFUNCTION(BlueprintCallable, Category="ObjectInteraction")
	void SetObjectPosition();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ObjectInteraction")
	int32 GrabDistance = 500;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ObjectInteraction")
	int32 ObjectDistanceFromCamera = 300;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ObjectInteraction")
	int32 MaxObjectDistanceFromCamera = 250;
		
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="CustomPlayer | Interaction")
	float RadiusSphereTrace = 10.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ObjectInteraction")
	bool bIsGrabbingObject = false;

	FVector GrabbingObjectLocation = FVector::Zero();
	
public:
	UPROPERTY(BlueprintAssignable, Category="ObjectInteraction")
	FOnHit OnHit;
	
	// ------------------- Rotation ------------------- //
protected:
	UFUNCTION(BlueprintCallable, Category="ObjectInteraction")
	void RotateObjectVectorIn(const FVector2D& Value);

	UFUNCTION(BlueprintCallable, Category="ObjectInteraction")
	void RotateObjectFloatIn(const float& Value);
	
	UFUNCTION(BlueprintCallable, Category="ObjectInteraction")
	void AddObjectRelativeRotation(const FRotator& InRotation);
	
	UFUNCTION(BlueprintCallable, Category="ObjectInteraction")
	void AddObjectWorldRotation(const FRotator& InRotation);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ObjectInteraction")
	float RotationRate = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ObjectInteraction")
	float MaxRotationRate = 5.0f;
	
	// ------------------- Setup Rotation ------------------- //
private:
	UFUNCTION()
	void SelectRotation();
	
	FRotator CameraRotation = FRotator::ZeroRotator;
	bool XOrientation = false;
	int ValueYInv = 0;
	int ValueInv = 0;
	
	// ------------------- Setup ------------------- //
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY()
	TObjectPtr<ACustomPlayer> Character = nullptr;
	
	TWeakObjectPtr<UCameraComponent> FirstPersonCameraComponent = nullptr;
};