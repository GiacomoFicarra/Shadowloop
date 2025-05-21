#include "GameOverComponent.h"

#include "CustomPlayer.h"
#include "Enemy.h"
#include "NavigationSystem.h"
#include "Kismet/KismetMathLibrary.h"

UGameOverComponent::UGameOverComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UGameOverComponent::IncreaseMovementJumpscareCounter()
{
	MovementJumpscareActionCounter++;

	if (MovementJumpscareActionCounter >= MovementJumpscareMaxActionsPerLevel[CurrentLevel])
	{
		ACustomPlayer* Player = Cast<ACustomPlayer>(GetOwner());
		if (!IsValid(Player)) { return; }

		
		// enemy spawn
		FNavLocation ResultLocation;
		UNavigationSystemV1* NavigationSystem = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());
		if (!IsValid(NavigationSystem)) { return; }
		const bool IsReachablePoint = GetReachablePoint(Player, ResultLocation , NavigationSystem);

		while (!IsReachablePoint)
		{
			GetReachablePoint(Player, ResultLocation, NavigationSystem);
		}
		
		ResultLocation.Location.Z = Player->GetActorLocation().Z;
		FRotator ResultRotator = Player->GetActorRotation();
		ResultRotator.Yaw += 180;
		AEnemy* Enemy = GetWorld()->SpawnActor<AEnemy>(EnemyClass, ResultLocation.Location, ResultRotator);
		OnMovementJumpscareMaxActions.Broadcast();
		Player->RotateCharacterToEnemy();
		MovementJumpscareActionCounter = 0;
		return;
	}
	
	if (MovementJumpscareActionCounter == static_cast<int>(MovementJumpscareMaxActionsPerLevel[CurrentLevel] * 0.75))
	{
		ACustomPlayer* Player = Cast<ACustomPlayer>(GetOwner());
		Player->ThirdMovementCounterEvent();
		return;
	}

	if (MovementJumpscareActionCounter == static_cast<int>(MovementJumpscareMaxActionsPerLevel[CurrentLevel] * 0.5))
	{
		ACustomPlayer* Player = Cast<ACustomPlayer>(GetOwner());
		Player->SecondMovementCounterEvent();
		return;
	}

	if (MovementJumpscareActionCounter == static_cast<int>(MovementJumpscareMaxActionsPerLevel[CurrentLevel] * 0.25))
	{
		ACustomPlayer* Player = Cast<ACustomPlayer>(GetOwner());
		Player->FirstMovementCounterEvent();
		return;
	}
}

bool UGameOverComponent::CheckDoorJumpscare()
{
	if (DoorJumpscareActionCounter > DoorJumpscareMaxActionsPerLevel[CurrentLevel])
	{
		DoorJumpscareActionCounter = 0;
		return false;
	}

	return true;
}

// Get a point for the AI to spawn and reach the player
bool UGameOverComponent::GetReachablePoint(ACustomPlayer* Player, FNavLocation& ResultLocation, UNavigationSystemV1* NavigationSystem)
{
	bool IsReachablePoint = NavigationSystem->GetRandomReachablePointInRadius(Player->GetActorLocation(), DistanceSpawnEnemy,
																		 ResultLocation);
	if (IsReachablePoint)
	{
		const double Distance = UKismetMathLibrary::Vector_Distance(ResultLocation, Player->GetActorLocation());
		IsReachablePoint = Distance > MinDistanceSpawnEnemy;
	}

	return IsReachablePoint;
}