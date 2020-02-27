// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
#include "GameFramework/Actor.h"


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

	InitialYaw = GetOwner()->GetActorRotation().Yaw; // if initialized with 10 will do an implicit conversion from integer 10 to float 10.0
	CurrentYaw = InitialYaw;
	TargetYaw = InitialYaw + 90.f;
/* 
	FRotator CurrentRotation = GetOwner()->GetActorRotation();

	CurrentRotation.Yaw += 90.f;

	GetOwner()->SetActorRotation(CurrentRotation); */
	
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UE_LOG(LogTemp, Warning, TEXT("%s"), *GetOwner()->GetActorRotation().ToString());
	UE_LOG(LogTemp, Warning, TEXT("Yaw is: %f"), GetOwner()->GetActorRotation().Yaw);

	CurrentYaw = FMath::Lerp(CurrentYaw, TargetYaw, 0.02);
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

