/**
 * Header Class for AI Guard
 */

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSAIGuard.generated.h"

// Forward declaring
class UPawnSensingComponent;
class ATargetPoint;

UENUM(BlueprintType)
enum class EAIState : uint8 {
	Idle, Suspicious, Alerted 
};

UCLASS()
class FPSGAME_API AFPSAIGuard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSAIGuard();

protected:

	/*
	 * Variables
	 */
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	// Used to set AI path
	UPROPERTY(EditAnywhere, Category = "AI")
	TArray<ATargetPoint*> PatrolPoints;
	
	// AI's current location
	ATargetPoint* CurrentPatrolPoint;
	
	// Counter for AI path
	int32 CurrentPatrolPointNum;

	// Time handlers
	FTimerHandle TimeHandler_ResetOrientation;
	FTimerHandle TimeHandler_Patrol;

	EAIState GuardState;

	FRotator OriginalRotation;

	UPROPERTY(EditAnywhere, Category = "AI")
	bool bCanPatrol;

	/*
	* Functions
	*/

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Binds to similar function
	UFUNCTION()
	void OnPawnSeen(APawn* SeenPawn);

	// Binds to similar function
	UFUNCTION()
	void OnSoundHeard(APawn* NoiseInstigator, const FVector& Location, float Volume);

	UFUNCTION()
	void ResetOrientation();

	void SetGuardState(EAIState GuardState);

	// Can be accessed in Blueprints
	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void OnStateChanged(EAIState GuardState);

	//Patrol methods
	void PatrolCheck();
	void MoveToNextPatrol();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
