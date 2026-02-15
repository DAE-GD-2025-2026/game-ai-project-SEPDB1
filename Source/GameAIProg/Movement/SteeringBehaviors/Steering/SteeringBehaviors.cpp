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
	const float Distance{ static_cast<float>(FVector2D::Distance(Agent.GetPosition(), Target.Position)) };
	float VelocityScale{ FMath::GetRangePct(Agent.GetSlowRadius(), Agent.GetTargetRadius(), Distance) };
	VelocityScale = FMath::Clamp(VelocityScale, 0.0f, 1.0f);
	
	GEngine->ClearOnScreenDebugMessages();
	GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Green, FString::Printf(TEXT("velocityScale = %f"), VelocityScale));
	
	Agent.SetLinearVelocityScale(VelocityScale);

	return Seek::CalculateSteering(DeltaT, Agent);
}
