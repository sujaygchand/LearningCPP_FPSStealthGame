/**
* Header Class for Black Hole
*
* Sucks up all primative components with physics enabled and destroys them
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSBlackHole.generated.h"

// Forward declaring
class USphereComponent;

UCLASS()
class FPSGAME_API AFPSBlackHole : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFPSBlackHole();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Mesh Component
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
		UStaticMeshComponent* MeshComp;

	// Inner sphere for overlap
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
		USphereComponent* InnerSphere;

	// Outer sphere for overlap
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
		USphereComponent* OuterSphere;

	// Blackhole Strength
	UPROPERTY(EditAnywhere, Category = "Defaults")
		float BlackHoleStrength = -4000;

	// Marked with Ufunction to bind overlap event
	UFUNCTION()
		void OverlapInnerSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
			int32 OtherBodyIndex, bool bIsFromSweep, const FHitResult& SweepResult);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
