/**
 * C++ Class for AI Guard
 */

#include "FPSAIGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "FPSCharacter.h"
#include "TimerManager.h"
#include "FPSGameMode.h"
#include "Engine/TargetPoint.h"
#include "AI/Navigation/NavigationSystem.h"
#include "EngineUtils.h"

 /*
 * Sets default values
 */
AFPSAIGuard::AFPSAIGuard() {
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Pawn Sensing Comp 
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	// Initial Guard state
	GuardState = EAIState::Idle;
}

/*
* Called when the game starts or when spawned
*/
void AFPSAIGuard::BeginPlay() {
	Super::BeginPlay();

	// Maps default AI events to my custom events
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnPawnSeen);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnSoundHeard);

	OriginalRotation = GetActorRotation();

	// Assign and execute if CanPatrol
	if (bCanPatrol)
	{
		CurrentPatrolPointNum = 0;
		CurrentPatrolPoint = PatrolPoints[CurrentPatrolPointNum];
		MoveToNextPatrol();
	}
}

/*
 * When a Pawn is seen
 */
void AFPSAIGuard::OnPawnSeen(APawn * SeenPawn) {

	// Is player check
	AFPSCharacter* MyPawn = Cast<AFPSCharacter>(SeenPawn);

	AFPSGameMode* GameMode = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		// Sends fail state to game mode
		GameMode->CompleteMission(SeenPawn, false);
	}

	// Debug
	if (MyPawn)
	{
		DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 30, 10, FColor::Emerald, false, 10);

		UE_LOG(LogTemp, Warning, TEXT("Seen Pawn %s"), *SeenPawn->GetName());
	}

	// Guard is now alerted
	GuardState = EAIState::Alerted;

}

/*
* When sound is heard
*/
void AFPSAIGuard::OnSoundHeard(APawn * NoiseInstigator, const FVector& Location, float Volume) {

	/*
	 * Sight and alert override this
	 */
	if (this->GuardState == EAIState::Alerted)
	{
		return;
	}

	// Debug
	DrawDebugSphere(GetWorld(), Location, 34, 10, FColor::Red, false, 10);


	FVector Direction = Location - GetActorLocation();
	Direction.Normalize();
	FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();

	NewLookAt.Pitch = GetActorRotation().Pitch;
	NewLookAt.Roll = GetActorRotation().Roll;

	// Looks at disturbance
	SetActorRotation(NewLookAt);

	// Resets timer and triggers event
	GetWorldTimerManager().ClearTimer(TimeHandler_ResetOrientation);
	GetWorldTimerManager().SetTimer(TimeHandler_ResetOrientation, this, &AFPSAIGuard::ResetOrientation, 3);

	SetGuardState(EAIState::Suspicious);

	// Need to stop moving guards
	if (GetController())
	{
		GetController()->StopMovement();
	}
}

/*
* Setter for Guard State
*/
void AFPSAIGuard::SetGuardState(EAIState GuardState) {

	if (GuardState == this->GuardState)
	{
		return;
	}
	else
	{
		this->GuardState = GuardState;
	}

	OnStateChanged(this->GuardState);
}

/*
* Checks if it can patrol
*/
void AFPSAIGuard::PatrolCheck() {

	if (!bCanPatrol)
	{
		return;
	}
	else
	{
		MoveToNextPatrol();
	}
}

/*
* Moves to Next Patrol Point
*/
void AFPSAIGuard::MoveToNextPatrol() {

	// Nullptr check
	if (CurrentPatrolPoint == nullptr)
	{
		return;
	}

	// Patrol only when Idle
	if (GuardState == EAIState::Idle)
	{
		FVector2D DeltaPatrolPoint = FVector2D(GetActorLocation().X - CurrentPatrolPoint->GetActorLocation().X,
			GetActorLocation().Y - CurrentPatrolPoint->GetActorLocation().Y);
		
		// Only go to next if close to its current goal
		if (FMath::Abs(DeltaPatrolPoint.Size()) < 40)
		{
			CurrentPatrolPointNum++;
		}
		
		// Sets point back to 0
		if (CurrentPatrolPointNum >= PatrolPoints.Num())
		{
			CurrentPatrolPointNum = 0;
		}

		// Moves AI
		CurrentPatrolPoint = PatrolPoints[CurrentPatrolPointNum];
		UNavigationSystem::SimpleMoveToActor(GetController(), CurrentPatrolPoint);

		// Resets timer and triggers event
		GetWorldTimerManager().ClearTimer(TimeHandler_Patrol);
		GetWorldTimerManager().SetTimer(TimeHandler_Patrol, this, &AFPSAIGuard::PatrolCheck, 5);
	}
}

/*
* Called every frame
*/
void AFPSAIGuard::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

/*
* Resets the guards Orientation when it is suspicious
*/
void AFPSAIGuard::ResetOrientation() {

	// Once Alerted the guards cannot be reset
	if (this->GuardState == EAIState::Alerted)
	{
		return;
	}

	// Resets Patrol pattern
	if (bCanPatrol)
	{
		GetWorldTimerManager().ClearTimer(TimeHandler_Patrol);
		GetWorldTimerManager().SetTimer(TimeHandler_Patrol, this, &AFPSAIGuard::PatrolCheck, 1);
	}

	// Resets original orientation
	SetActorRotation(OriginalRotation);
	SetGuardState(EAIState::Idle);
}




