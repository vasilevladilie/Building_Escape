// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandle();
	SetupInputComponent();

	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty!"));
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber press"));

	// TODO, to only raycast when the key is pressed and see if we reached any actors with physics body collision channel set.

	// If we hit something then attach the physics handle.
	// TODO attach physics handle
	
	FHitResult HitResult = GetFirstPhysicsBodyInReach();

	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint
	(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	FVector PlayerViewDirection = PlayerViewPointRotation.Vector() * Reach;
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewDirection;

	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();

	// if we hit something then attach the physics handle
	if (HitResult.GetActor())
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor hit - grab it"));
		// Attach physics handle
		PhysicsHandle->GrabComponentAtLocation
		(
			ComponentToGrab,
			NAME_Actor,
			LineTraceEnd
		);
		UE_LOG(LogTemp, Warning, TEXT("Actor hit - grabbed"));
	}

}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber released"));
	// TODO remove/release the physics handle
	PhysicsHandle->ReleaseComponent();
}

void UGrabber::FindPhysicsHandle()
{
	// Checking for Physics Handle Component
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle)
	{
		// Physics is found.

		// Found the Input Component

	}
	else
	{
		/* code */
		UE_LOG(LogTemp, Error, TEXT("No physics handle component found on %s!"), *GetOwner()->GetName());
	}
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Input Component found"), *GetOwner()->GetName());
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);

		// It is still about the Grab key but is about the release action
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Input Component MISSING"), *GetOwner()->GetName());
	}
}

/// THIS IS THE HOT LOOP!
// Called every frame 
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// if the physics handle is attached
	// move the object we are holding

	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint
	(
		OUT PlayerViewPointLocation,
	 	OUT PlayerViewPointRotation
	);

	FVector PlayerViewDirection = PlayerViewPointRotation.Vector() * Reach;
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewDirection;

	if (PhysicsHandle->GrabbedComponent)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Actor hit - move grabbed component"));
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
}

void UGrabber::ProjectRayCast()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	FVector LineTraceEnd = PlayerViewPointLocation + FVector(0.f/*Values are in centimeters*/, 0.f, 100.f);

	FVector PlayerViewDirection = PlayerViewPointRotation.Vector() * Reach;

	LineTraceEnd = PlayerViewPointLocation + PlayerViewDirection;

	DrawDebugLine(GetWorld(), PlayerViewPointLocation, LineTraceEnd, FColor(0, 255, 0), false, 0.f, 0, 5.f);
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	// Get Player's Viewpoint

	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	// Logging out to test

	// UE_LOG(LogTemp, Warning, TEXT("Player View Point Location: %s"), *(PlayerViewPointLocation.ToString()));

	// UE_LOG(LogTemp, Warning, TEXT("Player View Point Direction: %s"), *(PlayerViewPointRotation.ToString()));

	// Draw a line from player showing the reach.
	
	FHitResult Hit;
	// Ray-cast out to a certain distance (Reach)
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	FVector PlayerViewDirection = PlayerViewPointRotation.Vector() * Reach;

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewDirection;

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	// See what it hits
	AActor* RayCastHitActor = Hit.GetActor(); // Actor which meets the set criteria in LineTraceSingleByObjectType
	if (RayCastHitActor)
	{
		// Logging out what was hit
		UE_LOG(LogTemp, Warning, TEXT("Ray cast hit actor: %s"), *(RayCastHitActor->GetName()));
	}

	return Hit;
}