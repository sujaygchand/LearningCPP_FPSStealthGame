/**
* Header Class for Objective Actor
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSObjectiveActor.generated.h"

// Forward declaring
class USphereComponent;

UCLASS()
class FPSGAME_API AFPSObjectiveActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSObjectiveActor();

protected:


	// Mesh Component
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComp;
	
	// Sphere Component
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* SphereComp;

	// Particle variable
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* PickupFX;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void PlayEffects();

public:	
		
	// Begin Overlap signature 
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

};
