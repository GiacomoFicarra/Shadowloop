#pragma once

#include "CoreMinimal.h"
#include "CustomPlayer.h"
#include "Enemy.h"
#include "NavigationSystem.h"
#include "Components/ActorComponent.h"
#include "GameOverComponent.generated.h"

class AEnemyAIController;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMovementJumpscareMaxActions);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SHADOWLOOP_API UGameOverComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGameOverComponent();

	// ------------------- Game Over Counters ------------------- //
public:
	UFUNCTION()
	void IncreaseDoorJumpscareCounter() { DoorJumpscareActionCounter++; };

	UFUNCTION()
	void IncreaseMovementJumpscareCounter();

	UFUNCTION(BlueprintCallable, Category = "GameOver")
	void IncreaseCurrentLevel() { CurrentLevel++; };

	UFUNCTION(BlueprintCallable, Category = "GameOver")
	bool CheckDoorJumpscare();

	UFUNCTION(BlueprintCallable, Category = "GameOver")
	void ResetDoorJumpscareCounter() { DoorJumpscareActionCounter = 0; };

	UFUNCTION(BlueprintCallable, Category = "GameOver")
	void ResetMovementJumpscareCounter() { MovementJumpscareActionCounter = 0; };
	
	UPROPERTY()
	FOnMovementJumpscareMaxActions OnMovementJumpscareMaxActions;
	
private:
	UPROPERTY(EditAnywhere, Category = "GameOver")
	TArray<int> DoorJumpscareMaxActionsPerLevel;

	UPROPERTY(EditAnywhere, Category = "GameOver")
	TArray<int> MovementJumpscareMaxActionsPerLevel;
	
	UPROPERTY(VisibleAnywhere, Category = "GameOver")
	int DoorJumpscareActionCounter = 0;

	UPROPERTY(VisibleAnywhere, Category = "GameOver")
	int MovementJumpscareActionCounter = 0;

	// ------------------- Save ------------------- //
public:
	UFUNCTION(Category = "SaveGameOver")
	void SetCurrentLevel(const int InCurrentLevel) {CurrentLevel = InCurrentLevel;};

	UFUNCTION(BlueprintCallable, Category = "SaveGameOver")
	int GetCurrentLevel() const {return CurrentLevel; };
	
private:
	UPROPERTY(VisibleAnywhere, Category = "GameOver")
	int CurrentLevel = 0;

	// ------------------- Enemy Spawn ------------------- //
private:
	bool GetReachablePoint(ACustomPlayer* Player, FNavLocation& ResultLocation, UNavigationSystemV1* NavigationSystem);

	UPROPERTY(EditAnywhere, Category = "GameOver")
	double DistanceSpawnEnemy = 100.0f;

	UPROPERTY(EditAnywhere, Category = "GameOver")
	double MinDistanceSpawnEnemy = 50;

	UPROPERTY(EditAnywhere, Category = "GameOver")
	TSubclassOf<AEnemy> EnemyClass;

	UPROPERTY(EditAnywhere, Category = "GameOver")
	TSubclassOf<AEnemyAIController> EnemyController;
};
