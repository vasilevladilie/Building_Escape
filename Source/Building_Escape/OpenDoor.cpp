// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"

#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"

int32 UOpenDoor::NumberOfCallsToBeginPlay = 0;

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	InitDoor();

	++NumberOfCallsToBeginPlay;
	UE_LOG(LogTemp, Error, TEXT("%i."), NumberOfCallsToBeginPlay);

	if (PressurePlate == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%i. %s has the open door component on it, but pressureplate 1 not set."), NumberOfCallsToBeginPlay, *GetOwner()->GetName());
	}

	if (PressurePlate2 == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%i. %s has the open door component on it, but pressureplate 2 not set."), NumberOfCallsToBeginPlay, *GetOwner()->GetName());
	}

	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if ((PressurePlate != nullptr && PressurePlate->IsOverlappingActor(ActorThatOpens)) || (PressurePlate2 != nullptr && PressurePlate2->IsOverlappingActor(ActorThatOpens)))
	{
		OpenDoor(DeltaTime);
		// DoorLastOpened When the Door was opened
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
	else if (PressurePlate != nullptr || PressurePlate2 != nullptr)
	{
		// if the Door has been open longer than x seconds - CloseDelay or DoorCloseDelay
		float CurrentTime = GetWorld()->GetTimeSeconds();
		if (CurrentTime - DoorLastOpened > DoorCloseDelay)
		{
			CloseDoor(DeltaTime);
		}
	}

	// float TimeInSeconds = GetWorld()->GetTimeSeconds(); // seconds since the world was launched, can be used to make a timer
}

void UOpenDoor::InitDoor()
{
	InitialYaw = GetOwner()->GetActorRotation().Yaw; // if initialized with 10 will do an implicit conversion from integer 10 to float 10.0
	CurrentYaw = InitialYaw;
	TargetYaw = InitialYaw + 90.f;
	
	/* 
	FRotator CurrentRotation = GetOwner()->GetActorRotation();

	CurrentRotation.Yaw += 90.f;

	GetOwner()->SetActorRotation(CurrentRotation); 
	*/
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	// UE_LOG(LogTemp, Warning, TEXT("%s"), *GetOwner()->GetActorRotation().ToString());
	// UE_LOG(LogTemp, Warning, TEXT("Yaw is: %f"), GetOwner()->GetActorRotation().Yaw);

	CurrentYaw = FMath::Lerp(CurrentYaw, TargetYaw, DeltaTime * DoorOpenSpeed);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);

	// float CurrentYaw = GetOwner()->GetActorRotation().Yaw;
	// FRotator OpenDoor
	// Change Yaw of OpenDoor
	// FRotator OpenDoor(0.f, TargetYaw, 0.f);
	// OpenDoor.Yaw = FMath::FInterpConstantTo(CurrentYaw, TargetYaw, DeltaTime, 3.f);

	// 5 functions overloads
	// This will be used to piecemeal open the door
	// FMath::Lerp(/*StartingYaw, TargetYaw, 0-1*/);
	//GetOwner()->SetActorRotation(OpenDoor);
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	// UE_LOG(LogTemp, Warning, TEXT("%s"), *GetOwner()->GetActorRotation().ToString());
	// UE_LOG(LogTemp, Warning, TEXT("Yaw is: %f"), GetOwner()->GetActorRotation().Yaw);

	CurrentYaw = FMath::Lerp(CurrentYaw, InitialYaw, DeltaTime * DoorCloseSpeed);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);
}