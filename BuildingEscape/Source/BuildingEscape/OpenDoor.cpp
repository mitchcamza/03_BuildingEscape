// Copyright Mitch Campbell 2021.


#include "OpenDoor.h"
#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	// Find the owning actor
	Owner = GetOwner();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the Trigger Volume
	if (GetTotalMassOfActorsOnPlate() > 30.f)	// TODO: make into a parameter
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	// Check if it's time to close the door
	if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay)
	{
		CloseDoor();
	}
}


void UOpenDoor::OpenDoor()
{
	// Create a rotator and set the rotation
	Owner->SetActorRotation(FRotator(0.0f, OpenAngle, 0.0f));
}


void UOpenDoor::CloseDoor()
{
	// Create a rotator and set the rotation
	Owner->SetActorRotation(FRotator(0.0f, 90.0f, 0.0f));
}


float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;

	// Find all the overlapping actors
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	// Iterate through actors, finding their masses
	for (const auto* Actor : OverlappingActors)
	{
		// print the name of current overlapping actor
		UE_LOG(LogTemp, Warning, TEXT("%s on pressure plate"), *Actor->GetName());

		// add the individual masses together
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}


	return TotalMass;
}