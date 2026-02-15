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

SteeringOutput Flee::CalculateSteering(float DeltaT, ASteeringAgent& Agent)
{
	SteeringOutput Steering{ Seek::CalculateSteering(DeltaT, Agent) };
	
	Steering.LinearVelocity = -Steering.LinearVelocity;
	return Steering;
}

SteeringOutput Arrive::CalculateSteering(float DeltaT, ASteeringAgent& Agent)
{
	SteeringOutput Steering{ Seek::CalculateSteering(DeltaT, Agent) };
	const float Distance = Steering.LinearVelocity.Length();
	float VelocityScale{ FMath::GetRangePct(Agent.GetSlowRadius(), Agent.GetTargetRadius(), Distance) };
	VelocityScale = FMath::Clamp(VelocityScale, 0.0f, 1.0f);

	Agent.SetSpeedByScale(VelocityScale);

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
