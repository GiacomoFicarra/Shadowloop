#include "ShadowLoop/Public/ObjectInteractionComponent.h"

#include "CustomPlayer.h"
#include "Camera/CameraComponent.h"
#include "ShadowLoop/ShadowLoopCharacter.h"
#include "DrawDebugHelpers.h"
#include "MovableObject.h"
#include "StandObject.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"


void UObjectInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<ACustomPlayer>(GetOwner());

	checkf(Character, TEXT("No Character found"));
	if (!IsValid(Character)) { return; }

	FirstPersonCameraComponent = Character->GetFirstPersonCameraComponent();
}


void UObjectInteractionComponent::GrabObject(UPrimitiveComponent* ComponentToGrab, AActor* InActor,
                                             const FName InBoneName)
{
	if (!IsValid(ComponentToGrab)) { return; }
	if (!IsValid(InActor)) { return; }
	AMovableObject* ObjectToGrab = Cast<AMovableObject>(InActor);
	if (IsValid(ObjectToGrab))
	{
		GrabComponentAtLocationWithRotation(ComponentToGrab, InBoneName,
		                                    ComponentToGrab->GetComponentLocation(),
		                                    ComponentToGrab->GetComponentRotation());
		bIsGrabbingObject = true;
		OnHit.Broadcast(InActor);

		// ------------------- Prompt ------------------- //
		Character->OnObjectGrabbed.Broadcast(true);
	}
}


void UObjectInteractionComponent::SetObjectPosition()
{
	checkf(FirstPersonCameraComponent.IsValid(), TEXT("No Camera found"));
	if (bIsGrabbingObject)
	{
		GrabbingObjectLocation = FirstPersonCameraComponent->GetForwardVector() * ObjectDistanceFromCamera +
			FirstPersonCameraComponent->GetComponentLocation();
		SetTargetLocation(GrabbingObjectLocation);

		double Distance = UKismetMathLibrary::Vector_Distance(GrabbedComponent->GetComponentLocation(),
									FirstPersonCameraComponent->GetComponentLocation());
		if (Distance > MaxObjectDistanceFromCamera) { ReleaseObject();}
	}
}

void UObjectInteractionComponent::ReleaseObject()
{
	Character->OnObjectGrabbed.Broadcast(false);
	ReleaseComponent();
	CameraRotation = FRotator::ZeroRotator;
	bIsGrabbingObject = false;
}

void UObjectInteractionComponent::RotateObjectVectorIn(const FVector2D& Value)
{
	FRotator InputRotator = FRotator::ZeroRotator;
	InputRotator.Yaw = FMath::Clamp(Value.X * RotationRate, -MaxRotationRate, MaxRotationRate);

	if (XOrientation)
	{
		InputRotator.Pitch = FMath::Clamp(Value.Y * RotationRate * ValueYInv, -MaxRotationRate, MaxRotationRate);
	}
	else
	{
		InputRotator.Roll = FMath::Clamp(Value.Y * RotationRate * ValueYInv, -MaxRotationRate, MaxRotationRate);
	}
	
	
	AddObjectWorldRotation(InputRotator);
}

void UObjectInteractionComponent::RotateObjectFloatIn(const float& Value)
{
	FRotator InputRotator = FRotator::ZeroRotator;
	
	if (XOrientation)
	{
		InputRotator.Roll = FMath::Clamp(Value * RotationRate * ValueInv, -MaxRotationRate, MaxRotationRate);
	}
	else
	{
		InputRotator.Pitch = FMath::Clamp(Value * RotationRate * ValueInv, -MaxRotationRate, MaxRotationRate);
	}
	
	AddObjectWorldRotation(InputRotator);
}

void UObjectInteractionComponent::AddObjectRelativeRotation(const FRotator& InRotation)
{
	TargetTransform.SetRotation(TargetTransform.GetRotation() * InRotation.Quaternion());
}

void UObjectInteractionComponent::AddObjectWorldRotation(const FRotator& InRotation)
{
	FRotator WorldRotation = FRotator(InRotation.Quaternion() * TargetTransform.GetRotation());
	SetTargetRotation(WorldRotation);
}

bool UObjectInteractionComponent::GrabObjectWithLineTrace()
{
	checkf(FirstPersonCameraComponent.IsValid(), TEXT("No Camera found"));
	if (!FirstPersonCameraComponent.IsValid()) { return false; }
	FHitResult HitResult;
	FVector StartPoint = FirstPersonCameraComponent->GetComponentLocation();
	FVector EndPoint = FirstPersonCameraComponent->GetForwardVector() * GrabDistance + StartPoint;
	FCollisionQueryParams Params;
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this->GetOwner());
	Params.AddIgnoredActor(this->GetOwner());

	if (UKismetSystemLibrary::SphereTraceSingle(GetWorld(), StartPoint, EndPoint, RadiusSphereTrace,
												UEngineTypes::ConvertToTraceType(ECC_Visibility)
												, false, IgnoreActors,
												EDrawDebugTrace::ForDuration, HitResult, true))
	{
		AMovableObject* ObjectToGrab = Cast<AMovableObject>(HitResult.GetActor());
		if (!IsValid(ObjectToGrab))
		{
			return false;
		}

		if (!ObjectToGrab->GetIsAttached())
		{
			return false;
		}
		
		GrabComponentAtLocationWithRotation(HitResult.GetComponent(), HitResult.BoneName,
												HitResult.GetComponent()->GetComponentLocation(),
												HitResult.GetComponent()->GetComponentRotation());
		OnHit.Broadcast(HitResult.GetActor());

		// ------------------- Setup Rotation ------------------- //
		
		const UCameraComponent* StandCamera = ObjectToGrab->GetAttachedStand()->GetCamera();
		if (!IsValid(StandCamera))
		{
			return false;
		}
		
		CameraRotation = StandCamera->GetComponentRotation();
		SelectRotation();

		return true;
	}

	return false;
}

void UObjectInteractionComponent::SelectRotation()
{
	const float CameraYaw = CameraRotation.Yaw;

	const float CameraAngle = UKismetMathLibrary::Abs(CameraYaw);

	XOrientation = CameraAngle <= 45 || CameraAngle >= 135;
	
	if (CameraAngle <= 45)
	{
		ValueYInv = 1;
		ValueInv = -1;
	}
	else if (CameraAngle >= 135)
	{
		ValueYInv = -1;
		ValueInv = 1;
	}
	else
	{
		ValueYInv = -UKismetMathLibrary::SignOfFloat(CameraYaw);
		ValueInv = -UKismetMathLibrary::SignOfFloat(CameraYaw);
	}
}
