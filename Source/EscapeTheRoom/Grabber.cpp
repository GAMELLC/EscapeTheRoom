// Copyright Gaming Association of Malcontent's Entertainment 2018

#include "Grabber.h"

#define OUT


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	CheckPhysicsHandleComponent();
	CheckInputComponent();
	
}

void UGrabber::CheckPhysicsHandleComponent()
{
	UE_LOG(LogTemp, Warning, TEXT("CheckPhysicsHandleComponent()"))
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to locate component <UPhysicsHandleComponent> on %s"), *GetOwner()->GetName())
		return;
	}
}

void UGrabber::CheckInputComponent()
{
	UE_LOG(LogTemp, Warning, TEXT("CheckInputComponent()"))
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to locate component <UInputComponent> on %s"), *GetOwner()->GetName())
		return;
	}
	else
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab()"))
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	if(ActorHit)
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None, // no bones needed
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true // allow rotation
		);
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Release()"))
	PhysicsHandle->ReleaseComponent();
}

FVector UGrabber::GetTraceEndPoint()
{
	UE_LOG(LogTemp, Warning, TEXT("GetTraceEndPoint()"))
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}

FVector UGrabber::GetTraceStartPoint()
{
	UE_LOG(LogTemp, Warning, TEXT("GetTraceStartPoint()"))
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	return PlayerViewPointLocation;
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	UE_LOG(LogTemp, Warning, TEXT("GetFirstPhysicsBodyInReach()"))
	FHitResult HitResult;
	FCollisionQueryParams TraceParams = (FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		GetTraceStartPoint(),
		GetTraceEndPoint(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
		);
	// See what we hit
	AActor* ActorHit = HitResult.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line Trace Hit: %s "), *(ActorHit->GetName()))
	}
	return HitResult;
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!PhysicsHandle)
	{
		return;
	}
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetTraceEndPoint());
	}

}

