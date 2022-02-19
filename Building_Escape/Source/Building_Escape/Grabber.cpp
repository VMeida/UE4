// Copyright VMA 2021

#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	
	FindPhysicsHandle();
	SetupInputComponent();
}

// Check for physics handle component
void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
		UE_LOG(LogTemp,Warning, TEXT("Physics handle found"));
	}
	else
	{
		UE_LOG(LogTemp, Error,TEXT("Physics handle for %s not found"), *GetOwner()->GetName());
	}
}

// Check and setup for input component
void UGrabber::SetupInputComponent()
{
	InputHandle = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputHandle)

	{
		UE_LOG(LogTemp,Warning, TEXT("Input handle found"),*GetOwner()->GetName());
		InputHandle->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputHandle->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error,TEXT("Input handle for %s not found"), *GetOwner()->GetName());
	}
}

void UGrabber::Grab()
{
	FVector LineTraceEnd = GetLineTraceEnd();
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();

	// If we hit something then attach the phisics handle
	if (HitResult.GetActor())
	{
		PhysicsHandle->GrabComponentAtLocation
										(
											ComponentToGrab,
											NAME_None,
											LineTraceEnd
										);
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp,Warning,TEXT("Grabber released"));
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->ReleaseComponent();
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	FVector LineTraceEnd = GetLineTraceEnd();
	FHitResult Hit;

	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
		);

	// Ray-cast out to a certain distance (reach)
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);
	// See what it hits
	AActor* ActorHit = Hit.GetActor();
	// Loggin out to test
	if (ActorHit)
	{
		UE_LOG(LogTemp,Warning,TEXT("Object hit: %s"),*(ActorHit->GetName()));
	};

	return Hit;
}

FVector UGrabber::GetLineTraceEnd() const
{
	// Get the payer viewpoint 
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
		);

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	return LineTraceEnd;
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector LineTraceEnd = GetLineTraceEnd();

	// If the physics handle is attached.
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
		//Move the object we are holding
}