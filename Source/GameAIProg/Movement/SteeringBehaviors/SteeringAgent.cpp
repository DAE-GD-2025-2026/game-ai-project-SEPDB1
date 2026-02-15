// Fill out your copyright notice in the Description page of Project Settings.

#include "SteeringAgent.h"

#include "Interfaces/IPluginManager.h"

const float ASteeringAgent::DefaultSpeed{ 600.f };

// Sets default values
ASteeringAgent::ASteeringAgent()
	: SlowRadius{ 100.f }
	, TargetRadius{ 500.f }
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;
	
}

// Called when the game starts or when spawned
void ASteeringAgent::BeginPlay()
{
	Super::BeginPlay();
	SetMaxLinearSpeed(DefaultSpeed);
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
		AddMovementInput(FVector{output.LinearVelocity, 0.f});
	}
	
	//GEngine->ClearOnScreenDebugMessages();
	//GEngine->AddOnScreenDebugMessage(-1, 0.05f, FColor::Yellow, FString::Printf(TEXT("z = %f"), GetActorRotation().Euler().Z));
	//GEngine->AddOnScreenDebugMessage(-1, 0.05f, FColor::Blue, FString::Printf(TEXT("Angular Velocity = %f"), GetAngularVelocity()));
	//GEngine->AddOnScreenDebugMessage(-1, 0.05f, FColor::Purple, FString::Printf(TEXT("Yaw = %f"), GetActorRotation().Yaw));
}

// Called to bind functionality to input
void ASteeringAgent::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ASteeringAgent::SetSteeringBehavior(ISteeringBehavior* NewSteeringBehavior)
{
	SteeringBehavior = NewSteeringBehavior;
	SetMaxLinearSpeed(DefaultSpeed);
}

void ASteeringAgent::SetSpeedByScale(const float Scale)
{
	SetMaxLinearSpeed(DefaultSpeed * Scale);
}

float ASteeringAgent::GetSlowRadius() const
{
	return SlowRadius;
}

float ASteeringAgent::GetTargetRadius() const
{
	return TargetRadius;
}
