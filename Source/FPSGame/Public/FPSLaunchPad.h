/**
* Header Class for Black Hole
* 
* Jump pad
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSLaunchPad.generated.h"

// Forward declaring
class UBoxComponent;

UCLASS()
class FPSGAME_API AFPSLaunchPad : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFPSLaunchPad();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Mesh Component
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UStaticMeshComponent* MeshComp;

	// Overlap Component
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UBoxComponent* OverlapComp;

	// JumpPad Target actor
	UPROPERTY(EditAnywhere, Category = "LandingTarget")
	AActor* LandingTarget;

	// Marked with Ufunction tobind overlap event
	UFUNCTION()
		void OverlapJumpPad(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bIsFromSweep, const FHitResult& SweepResult);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;



};
