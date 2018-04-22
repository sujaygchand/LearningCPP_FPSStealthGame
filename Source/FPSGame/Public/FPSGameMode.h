/**
* Header Class for Game Mode
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPSGameMode.generated.h"

UCLASS()
class AFPSGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Inspecting")
		TSubclassOf<AActor> SpectatingActorVP;

public:

	AFPSGameMode();

	// Mission Complete function C++
	void CompleteMission(APawn* InstigatorPawn, bool bMissionSuccess);

	// Mission Complete function Blueprint
	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void OnMissionCompleted(APawn* InstigatorPawn, bool bMissionSuccess);
};



