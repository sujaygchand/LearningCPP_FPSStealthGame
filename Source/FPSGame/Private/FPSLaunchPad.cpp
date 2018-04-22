/**
* Header Class for Black Hole
*
* Jump pad
*/

#include "FPSLaunchPad.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "FPSCharacter.h"

// Sets default values
AFPSLaunchPad::AFPSLaunchPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Mesh Comp
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	MeshComp->SetCollisionResponseToAllChannels(ECR_Block);
	RootComponent = MeshComp;

	//Overlap Comp
	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));
	OverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapComp->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);
	OverlapComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	OverlapComp->SetupAttachment(RootComponent);

	// Binds Overlap event
	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSLaunchPad::OverlapJumpPad);

}

/*
* Called when the game starts or when spawned
*/
void AFPSLaunchPad::BeginPlay()
{
	Super::BeginPlay();	
}

/*
 * Launches player upwards in the direction of its target
 *
 */
void AFPSLaunchPad::OverlapJumpPad(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp,
	int32 OtherBodyIndex, bool bIsFromSweep, const FHitResult & SweepResult) {

	AFPSCharacter* MyPlayer = Cast<AFPSCharacter>(OtherActor);

	FVector LaunchVelocity = (LandingTarget->GetActorLocation()) - GetActorLocation() + FVector(0, 0, 600);

	if (MyPlayer && LandingTarget)
	{
		MyPlayer->LaunchCharacter(LaunchVelocity, true, true);
	}
		else if (OtherComp && OtherComp->IsSimulatingPhysics())
		{
			OtherComp->AddImpulse(LaunchVelocity,NAME_Name, true);
			}
}

/*
*  Called every frame
*/
void AFPSLaunchPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

