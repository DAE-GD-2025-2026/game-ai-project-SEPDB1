#include "SteeringBehaviors.h"

#include <string>

#include "GameAIProg/Movement/SteeringBehaviors/SteeringAgent.h"

//SEEK
//*******
// TODO: Do the Week01 assignment :^)

SteeringOutput Seek::CalculateSteering(float DeltaT, ASteeringAgent& Agent)
{
	SteeringOutput Steering{};
	Steering.LinearVelocity = Target.Position - Agent.GetPosition();
	
	return Steering;
}

const float Seek::DefaultSpeed{ 600.f };

SteeringOutput Flee::CalculateSteering(float DeltaT, ASteeringAgent& Agent)
{
	SteeringOutput Steering{ Seek::CalculateSteering(DeltaT, Agent) };
	
	Steering.LinearVelocity = -Steering.LinearVelocity;
	return Steering;
}

Arrive::Arrive()
	: SlowRadius{ 100.f }
	, TargetRadius{ 500.f }
{
}

SteeringOutput Arrive::CalculateSteering(float DeltaT, ASteeringAgent& Agent)
{
	SteeringOutput Steering{ Seek::CalculateSteering(DeltaT, Agent) };
	const float Distance = FVector2D::Distance(Agent.GetPosition(), Target.Position);
	float VelocityScale{ FMath::GetRangePct(SlowRadius, TargetRadius, Distance) };
	VelocityScale = FMath::Clamp(VelocityScale, 0.0f, 1.0f);

	Agent.SetMaxLinearSpeed(VelocityScale * Seek::GetDefaultSpeed());
	

#pragma region DrawDebug
	DrawDebugCircle(
		Agent.GetWorld(), 
		FVector(Agent.GetPosition(), 0.f), 
		SlowRadius, 
		32, 
		FColor::Yellow, 
		false, 
		-1, 
		0, 
		0, 
		FVector::RightVector, 
		FVector::ForwardVector);
	
	DrawDebugCircle(
		Agent.GetWorld(), 
		FVector(Agent.GetPosition(), 0.f), 
		TargetRadius, 
		32, 
		FColor::Blue, 
		false, 
		-1, 
		0, 
		0, 
		FVector::RightVector, 
		FVector::ForwardVector);
	
#pragma endregion DrawDebug
	
	return Seek::CalculateSteering(DeltaT, Agent);
}

SteeringOutput Face::CalculateSteering(float DeltaT, ASteeringAgent& Agent)
{
	Agent.SetMaxLinearSpeed(0);
	return Seek::CalculateSteering(DeltaT, Agent);
}

SteeringOutput Pursuit::CalculateSteering(float DeltaT, ASteeringAgent& Agent)
{
	SteeringOutput Steering{ Seek::CalculateSteering(DeltaT, Agent) };
	const float TimeAwayFromTarget = Steering.LinearVelocity.Length() / Agent.GetMaxLinearSpeed();
	//const float TargetSpeed = Target
	
	return Steering;
}

Wander::Wander()
	: Offset{ 125.0 }
	, Radius{ 100.f }
	, CalculatedAngle{ 0.f }
{
}

SteeringOutput Wander::CalculateSteering(float DeltaT, ASteeringAgent& Agent)
{
	const float RandAngle{ FMath::RandRange(0.f, PI * 2.f)};
	const FVector2D RandOffset{ FVector2d(FMath::Cos(RandAngle), FMath::Sin(RandAngle)) * Radius };
	
	SetTarget(FTargetData(Agent.GetPosition() + RandOffset));
	
#pragma region DrawDebug
	FVector OffsetCenter{ Agent.GetActorLocation() + Agent.GetActorForwardVector() * Offset };
	
	DrawDebugCircle(
	Agent.GetWorld(), 
	OffsetCenter, 
	Radius, 
	32, 
	FColor::Yellow, 
	false, 
	-1, 
	0, 
	0, 
	FVector::RightVector, 
	FVector::ForwardVector);
	
	DrawDebugLine(
		Agent.GetWorld(), 
		Agent.GetActorLocation(), 
		FVector(Target.Position, 0.f), 
		FColor::Emerald);
	
	DrawDebugLine(
		Agent.GetWorld(), 
		Agent.GetActorLocation(), 
		OffsetCenter, 
		FColor::Magenta);
#pragma endregion DrawDebug
	
	return Seek::CalculateSteering(DeltaT, Agent);
}
