// Fill out your copyright notice in the Description page of Project Settings.

#include "SteeringAgent.h"

#include "AIController.h"
#include "Interfaces/IPluginManager.h"

//const float ASteeringAgent::DefaultSpeed{ 600.f };

// Sets default values
ASteeringAgent::ASteeringAgent()
	// : SlowRadius{ 100.f }
	// , TargetRadius{ 500.f }
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ABaseAgent::SetIsAutoOrienting(false);
}

// Called when the game starts or when spawned
void ASteeringAgent::BeginPlay()
{
	Super::BeginPlay();
}

void ASteeringAgent::BeginDestroy()
{
	Super::BeginDestroy();
}

// Called every frame
void ASteeringAgent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (SteeringBehavior)
	{
		SteeringOutput output = SteeringBehavior->CalculateSteering(DeltaTime, *this);
		
		if (!output.IsValid)
			return;
		
		AddMovementInput((FVector(output.LinearVelocity, 0.f)));
		AAIController* AIController = Cast<AAIController>(GetController());
		
		if (!IsAutoOrienting() && AIController)
		{
			float const DeltaYaw = FMath::Clamp(output.AngularVelocity, -1.f, 1.f) * GetMaxAngularSpeed() * DeltaTime;
				
			FRotator const CurrentRotation{ GetActorForwardVector().ToOrientationRotator() };
			FRotator const DeltaRotation{ 0.f, DeltaYaw, 0.f };
			FRotator const DesiredRotation{ CurrentRotation + DeltaRotation };
				
			if (!FMath::IsNearlyEqual(CurrentRotation.Yaw, DesiredRotation.Yaw))
			{
				AIController->SetControlRotation(DesiredRotation);
				FaceRotation(DesiredRotation);
			}
		}
	}
}

// Called to bind functionality to input
void ASteeringAgent::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ASteeringAgent::SetSteeringBehavior(ISteeringBehavior* NewSteeringBehavior)
{
	SteeringBehavior = NewSteeringBehavior;
}
