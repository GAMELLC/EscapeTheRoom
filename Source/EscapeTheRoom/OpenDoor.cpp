// Copyright Gaming Association of Malcontent's Entertainment 2018

#include "OpenDoor.h"

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

	Owner = GetOwner();

	if (!PressurePlate) { UE_LOG(LogTemp, Error, TEXT("Pressure Plate Unassigned on %s"), *GetOwner()->GetName()) }
	if (!Owner)
	{
		UE_LOG(LogTemp, Error, TEXT("Owned Failed to Assign!"))
	}
}

//returns total mass in kilograms
float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;

	//find all overlapping actors
	TArray<AActor*> OverlappingActors;
	if (!PressurePlate) {return TotalMass;}
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	//iterate through adding masses
	for (const auto* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	return TotalMass;
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);



	// Poll the Trigger Volume
	// If the ActorThatOpens is in the volume
	if (GetTotalMassOfActorsOnPlate() >= TriggerMass) // TODO make into a parameter
	{
		OnOpen.Broadcast();
	}

	if (GetTotalMassOfActorsOnPlate() <= TriggerMass)
	{
		OnClose.Broadcast();
	}
}
