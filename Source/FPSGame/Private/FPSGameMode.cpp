/**
* C++ Class for Game Mode
*/

#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

AFPSGameMode::AFPSGameMode() {
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();
}

/*
 * Ends the mission and changes the view
 */
void AFPSGameMode::CompleteMission(APawn * InstigatorPawn, bool bMissionSuccess) {

	if (InstigatorPawn)
	{
		InstigatorPawn->DisableInput(nullptr);

		if (SpectatingActorVP)
		{
			TArray<AActor*> ReturnedActors;
			UGameplayStatics::GetAllActorsOfClass(this, SpectatingActorVP, ReturnedActors);

			if (ReturnedActors.Num() > 0)
			{
				AActor* NewViewTarget = ReturnedActors[0];

				APlayerController* PlayerController = Cast<APlayerController>(InstigatorPawn->GetController());

				if (PlayerController)
				{
					PlayerController->SetViewTargetWithBlend(NewViewTarget, 0.5f, VTBlend_Cubic);
				}
			}
		}

		else
		{
			UE_LOG(LogTemp, Warning, TEXT("SpectatingActorVP is nullptr. Please update GameMode class"));
		}
	}
	OnMissionCompleted(InstigatorPawn, bMissionSuccess);
}