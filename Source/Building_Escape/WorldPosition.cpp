// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldPosition.h"
#include <EngineGlobals.h>
#include <Runtime/Engine/Classes/Engine/Engine.h>
#include "UnrealString.h"

// Sets default values for this component's properties
UWorldPosition::UWorldPosition()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWorldPosition::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("This is a Warning!"));	
	UE_LOG(LogTemp, Error, TEXT("Error!"));	
	UE_LOG(LogTemp, Display, TEXT("Just a log nothing to see!"));	

	FString ObjectName = GetOwner()->GetName();

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *ObjectName);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Vlad's Debug message Printed on Screen!"));

	FVector ActorLocation = GetOwner()->GetTransform().GetLocation();
	
	FString PrintActorLocation = FString::Printf(TEXT("Actor %s location x : %f, y: %f, z: %f"), *ObjectName, ActorLocation.X, ActorLocation.Y, ActorLocation.Z);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, PrintActorLocation);

}


// Called every frame
void UWorldPosition::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

