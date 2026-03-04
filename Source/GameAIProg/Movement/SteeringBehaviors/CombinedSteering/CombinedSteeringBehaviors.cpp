
#include "CombinedSteeringBehaviors.h"
#include <algorithm>
#include "../SteeringAgent.h"

BlendedSteering::BlendedSteering(const std::vector<WeightedBehavior>& WeightedBehaviors)
	: WeightedBehaviors(WeightedBehaviors)
{};

//****************
//BLENDED STEERING
SteeringOutput BlendedSteering::CalculateSteering(float DeltaT, ASteeringAgent& Agent)
{
	SteeringOutput BlendedSteering = {};
	//TODO: Calculate the weighted average steeringbehavior
	float  WeightedAverage{ 0.f };
	
	for (const WeightedBehavior& WeightedBehavior : WeightedBehaviors)
	{
		SteeringOutput Steering{ CalculateSteering(DeltaT, Agent) };
		
		BlendedSteering.LinearVelocity = Steering.LinearVelocity * WeightedBehavior.Weight;
		BlendedSteering.AngularVelocity = Steering.AngularVelocity * WeightedBehavior.Weight;
		
		WeightedAverage += WeightedBehavior.Weight;
	}
	
	BlendedSteering.LinearVelocity /= WeightedAverage;
	BlendedSteering.AngularVelocity /= WeightedAverage;
	
#pragma region DebugDrawing
	if (Agent.GetDebugRenderingEnabled())
		DrawDebugDirectionalArrow(
			Agent.GetWorld(),
			Agent.GetActorLocation(),
			Agent.GetActorLocation() + FVector{BlendedSteering.LinearVelocity, 0} * (Agent.GetMaxLinearSpeed() * DeltaT),
			30.f, FColor::Red);
#pragma endregion DebugDrawing

	return BlendedSteering;
}

float* BlendedSteering::GetWeight(ISteeringBehavior* const SteeringBehavior)
{
	auto it = find_if(WeightedBehaviors.begin(),
		WeightedBehaviors.end(),
		[SteeringBehavior](const WeightedBehavior& Elem)
		{
			return Elem.pBehavior == SteeringBehavior;
		}
	);

	if(it!= WeightedBehaviors.end())
		return &it->Weight;
	
	return nullptr;
}

//*****************
//PRIORITY STEERING
SteeringOutput PrioritySteering::CalculateSteering(float DeltaT, ASteeringAgent& Agent)
{
	SteeringOutput Steering = {};

	for (ISteeringBehavior* const pBehavior : m_PriorityBehaviors)
	{
		Steering = pBehavior->CalculateSteering(DeltaT, Agent);
	
		if (Steering.IsValid)
			break;
	}

	//If non of the behavior return a valid output, last behavior is returned
	return Steering;
}

