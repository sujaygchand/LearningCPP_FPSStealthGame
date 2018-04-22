/**
* C++ Class for Black Hole
*
* Sucks up all primative components with physics enabled and destroys them
*/

#include "FPSBlackHole.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"

// Sets default values
AFPSBlackHole::AFPSBlackHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Mesh comp
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;

	// Inner sphere comp
	InnerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InnerSphere"));
	InnerSphere->SetSphereRadius(160);
	InnerSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InnerSphere->SetCollisionResponseToAllChannels(ECR_Overlap);
	InnerSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	InnerSphere->SetupAttachment(MeshComp);
	
	// Maps default overlap event to my custom event
	InnerSphere->OnComponentBeginOverlap.AddDynamic(this, &AFPSBlackHole::OverlapInnerSphere) ;

	// Outer sphere comp
	OuterSphere = CreateDefaultSubobject<USphereComponent>(TEXT("OuterSphere"));
	OuterSphere->SetSphereRadius(3000);
	OuterSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OuterSphere->SetCollisionResponseToAllChannels(ECR_Overlap);
	OuterSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	OuterSphere->SetupAttachment(MeshComp);
}

/*
 * Called when the game starts or when spawned
*/
void AFPSBlackHole::BeginPlay()
{
	Super::BeginPlay();
	
}

/*
* Destroys any object that overlaps with the inside of the black hole
*/
void AFPSBlackHole::OverlapInnerSphere(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor,
	UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bIsFromSweep, const FHitResult & SweepResult) {

	if (OtherActor != this)
	{
		OtherActor->Destroy();
	}
}

/*
*  Called every frame
*/
void AFPSBlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Finds all overlapping components that can collide and are physically simulated
	TArray<UPrimitiveComponent*> OverlappingComps;
	OuterSphere->GetOverlappingComponents(OverlappingComps);


	for (int32 i = 0; i < OverlappingComps.Num(); i++)
	{
		UPrimitiveComponent* tempPrimObj = OverlappingComps[i];

		// Debug
		GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Blue, tempPrimObj->GetName());

		//Pulls physics bodies that are within the outer sphere into the center of the Black Hole
		if (tempPrimObj->IsSimulatingPhysics() && tempPrimObj->IsGravityEnabled())
		{
			tempPrimObj->SetEnableGravity(false);
			tempPrimObj->AddRadialForce(GetActorLocation(), OuterSphere->GetScaledSphereRadius(), BlackHoleStrength, ERadialImpulseFalloff::RIF_Constant, true);
		}
			}
			
}


