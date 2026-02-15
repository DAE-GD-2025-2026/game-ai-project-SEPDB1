// Fill out your copyright notice in the Description page of Project Settings.

#include "SteeringAgent.h"

const float ASteeringAgent::DefaultLinearVelocityScale{ 0.001f };

// Sets default values
ASteeringAgent::ASteeringAgent()
	: SlowRadius{ 100.f }
	, TargetRadius{ 500.f }
	, LinearVelocityScale{ DefaultLinearVelocityScale }
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
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
	
	DrawDebugCircle(
		GetWorld(), 
		ABaseAgent::GetActorLocation(), 
		GetSlowRadius(), 
		32, 
		FColor::Yellow, 
		false, 
		-1, 
		0, 
		0, 
		FVector::RightVector, 
		FVector::ForwardVector);
	
	DrawDebugCircle(
		GetWorld(), 
		ABaseAgent::GetActorLocation(), 
		GetTargetRadius(), 
		32, 
		FColor::Blue, 
		false, 
		-1, 
		0, 
		0, 
		FVector::RightVector, 
		FVector::ForwardVector);
	
	if (SteeringBehavior)
	{
		SteeringOutput output = SteeringBehavior->CalculateSteering(DeltaTime, *this);
		AddMovementInput(FVector{output.LinearVelocity, 0.f}, LinearVelocityScale);
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
	LinearVelocityScale = DefaultLinearVelocityScale;
}

void ASteeringAgent::SetLinearVelocityScale(const float Scale)
{
	LinearVelocityScale = DefaultLinearVelocityScale * Scale;
}

float ASteeringAgent::GetSlowRadius() const
{
	return SlowRadius;
}

float ASteeringAgent::GetTargetRadius() const
{
	return TargetRadius;
}
