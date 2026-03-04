#include "SteeringBehaviors.h"

#include <string>

#include "GameAIProg/Movement/SteeringBehaviors/SteeringAgent.h"
#include "Runtime/Renderer/Internal/MaterialCache/MaterialCacheRenderer.h"

//SEEK
//*******
// TODO: Do the Week01 assignment :^)

SteeringOutput Seek::CalculateSteering(float DeltaT, ASteeringAgent& Agent)
{
	SteeringOutput Steering{};
	Steering.LinearVelocity = Target.Position - Agent.GetPosition();
	
#pragma region DrawDebug
	if (Agent.GetDebugRenderingEnabled())
	{
		const FVector LocationAgent{ FVector(Agent.GetPosition(), 0.f) };
		
		DrawDebugCircle(
			Agent.GetWorld(), 
			FVector(Target.Position, 0.f), 
			DebugRadiusTarget, 
			32, 
			FColor::Red, 
			false, 
			-1, 
			0, 
			0, 
			FVector::RightVector, 
			FVector::ForwardVector);
		
		DrawDebugLine(
			Agent.GetWorld(), 
			LocationAgent, 
			LocationAgent + FVector(Steering.LinearVelocity, 0.f), 
			FColor::Yellow);
	}
#pragma endregion DrawDebug
	
	return Steering;
}

SteeringOutput Flee::CalculateSteering(float DeltaT, ASteeringAgent& Agent)
{
	SteeringOutput Steering{};
	Steering.LinearVelocity = Target.Position - Agent.GetPosition();
	
#pragma region DrawDebug
	if (Agent.GetDebugRenderingEnabled())
	{
		const FVector LocationAgent{ FVector(Agent.GetPosition(), 0.f) };
		
		DrawDebugCircle(
			Agent.GetWorld(), 
			FVector(Target.Position, 0.f), 
			DebugRadiusTarget, 
			32, 
			FColor::Red, 
			false, 
			-1, 
			0, 
			0, 
			FVector::RightVector, 
			FVector::ForwardVector);
		
		DrawDebugLine(
			Agent.GetWorld(), 
			LocationAgent, 
			LocationAgent + FVector(Steering.LinearVelocity, 0.f), 
			FColor::Yellow);
	}
#pragma endregion DrawDebug
	
	return Steering;
}

SteeringOutput Arrive::CalculateSteering(float DeltaT, ASteeringAgent& Agent)
{
	SteeringOutput Steering{ Seek::CalculateSteering(DeltaT, Agent) };
	const float Distance = FVector2D::Distance(Agent.GetPosition(), Target.Position);
	float VelocityScale{ FMath::GetRangePct(SlowRadius, TargetRadius, Distance) };
	VelocityScale = FMath::Clamp(VelocityScale, 0.0f, 1.0f);

	Agent.SetMaxLinearSpeed(VelocityScale * Seek::GetDefaultSpeed());
	

#pragma region DrawDebug
	if (Agent.GetDebugRenderingEnabled())
	{
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
	}
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
	const FVector2D PursuerPos = Agent.GetPosition();
	const FVector2D PursuerVel = FVector2D(Agent.GetVelocity().X, Agent.GetVelocity().Y);
	const FVector2D TargetPos = Target.Position;
	const FVector2D TargetVel = FVector2D(Target.LinearVelocity.X, Target.LinearVelocity.Y);

	const float Distance = FVector2D::Distance(PursuerPos, TargetPos);
	const float PursuerSpeed = PursuerVel.Size();
	const float PredictionTime = PursuerSpeed > 0 ? Distance / PursuerSpeed : 0;

	Target.Position = TargetPos + TargetVel * PredictionTime;

	return Seek::CalculateSteering(DeltaT, Agent);
}

SteeringOutput Evade::CalculateSteering(float DeltaT, ASteeringAgent& Agent)
{
	const FVector2D PursuerPos = Agent.GetPosition();
	const FVector2D PursuerVel = FVector2D(Agent.GetVelocity().X, Agent.GetVelocity().Y);
	const FVector2D TargetPos = Target.Position;
	const FVector2D TargetVel = FVector2D(Target.LinearVelocity.X, Target.LinearVelocity.Y);

	const float Distance = FVector2D::Distance(PursuerPos, TargetPos);
	const float PursuerSpeed = PursuerVel.Size();
	const float PredictionTime = PursuerSpeed > 0 ? Distance / PursuerSpeed : 0;

	Target.Position = TargetPos + TargetVel * PredictionTime;

	return Flee::CalculateSteering(DeltaT, Agent);
}

Wander::Wander()
	: Offset{ 225.0 }
	, Radius{ 100.f }
	, CalculatedAngle{ 0.f }
{
}

SteeringOutput Wander::CalculateSteering(float DeltaT, ASteeringAgent& Agent)
{
	const float RandAngle{ FMath::RandRange(0.f, PI * 2.f)};
	const FVector2D CenterCircle = Agent.GetPosition() + FVector2D(Agent.GetActorForwardVector().X, Agent.GetActorForwardVector().Y) * Radius;
	const FVector2D RandomPoint{ CenterCircle + (FVector2D(FMath::Cos(RandAngle), FMath::Sin(RandAngle)) * Radius) };
	const FTargetData NewTarget{ RandomPoint };
	
	SetTarget(NewTarget);
	
#pragma region DrawDebug
	if (Agent.GetDebugRenderingEnabled())
	{
		DrawDebugCircle(
		Agent.GetWorld(), 
		FVector(CenterCircle, 0.0f), 
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
			FVector(Agent.GetActorLocation().X, Agent.GetActorLocation().Y, 0.f), 
			FVector(RandomPoint, 0.f), 
			FColor::Emerald);
	
		DrawDebugLine(
			Agent.GetWorld(), 
			FVector(Agent.GetActorLocation().X, Agent.GetActorLocation().Y, 0.f), 
			FVector(CenterCircle, 0.0f), 
			FColor::Magenta);	
	}
#pragma endregion DrawDebug
	
	return Seek::CalculateSteering(DeltaT, Agent);
}
