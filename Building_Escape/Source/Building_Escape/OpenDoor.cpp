// Copyright VMA 2021

#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
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
	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	// OpenAngle = InitialYaw + OpenAngle;
	OpenAngle += InitialYaw;

	if (!PressurePlate)
	{
		UE_LOG(LogTemp,Error,TEXT("%s has no pressure plane selected"),*GetOwner()->GetName());
	}

	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PressurePlate && PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
	else if (PressurePlate && !PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		if (GetWorld()->GetTimeSeconds() - DoorCloseDelay > DoorLastOpened)
		{
			CloseDoor(DeltaTime);
		}
	}
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	FRotator OpenDoor(0.f,OpenAngle,0.f);
	CurrentYaw = GetOwner()->GetActorRotation().Yaw;
	OpenDoor.Yaw = FMath::FInterpTo(CurrentYaw,OpenAngle,DeltaTime,DoorOpenSpeed);

	GetOwner()->SetActorRotation(OpenDoor);
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	FRotator CloseDoor(0.f,InitialYaw,0.f);
	CurrentYaw = GetOwner()->GetActorRotation().Yaw;
	CloseDoor.Yaw = FMath::FInterpTo(CurrentYaw,InitialYaw,DeltaTime,DoorCloseSpeed);

	GetOwner()->SetActorRotation(CloseDoor);
}