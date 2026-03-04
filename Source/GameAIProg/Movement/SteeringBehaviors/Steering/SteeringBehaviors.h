#pragma once

#include <Movement/SteeringBehaviors/SteeringHelpers.h>
#include "Kismet/KismetMathLibrary.h"

class ASteeringAgent;

// SteeringBehavior base, all steering behaviors should derive from this.
class ISteeringBehavior
{
public:
	ISteeringBehavior() = default;
	virtual ~ISteeringBehavior() = default;

	// Override to implement your own behavior
	virtual SteeringOutput CalculateSteering(float DeltaT, ASteeringAgent & Agent) = 0;

	void SetTarget(const FTargetData& NewTarget) { Target = NewTarget; }
	
	template<class T, std::enable_if_t<std::is_base_of_v<ISteeringBehavior, T>>* = nullptr>
	T* As()
	{ return static_cast<T*>(this); }

protected:
	FTargetData Target;
};

// Your own SteeringBehaviors should follow here...

class Seek : public ISteeringBehavior
{
public:
	virtual SteeringOutput CalculateSteering(float DeltaT, ASteeringAgent & Agent) override;
	virtual float GetDefaultSpeed() { return DefaultSpeed; }
	void SetTargetRadius(float NewTargetRadius) { m_TargetRadius = NewTargetRadius; }
	
private:
	float DefaultSpeed{ 600.f };
	float DebugRadiusTarget{ 15.f };
	float m_TargetRadius{ 100000.f  };
};

class Flee :  public ISteeringBehavior
{
public:
	virtual SteeringOutput CalculateSteering(float DeltaT, ASteeringAgent & Agent) override;
	float DebugRadiusTarget{ 15.f };
};

class Arrive : public Seek
{
public:
	virtual SteeringOutput CalculateSteering(float DeltaT, ASteeringAgent & Agent) override;
	
private:
	float SlowRadius{ 100.f };
	float TargetRadius{ 500.f };
	float DebugRadiusTarget{ 15.f };
};

class Face : public Seek
{
public:
	virtual SteeringOutput CalculateSteering(float DeltaT, ASteeringAgent & Agent) override;
	
private:
	float DebugRadiusTarget{ 15.f };
};

class Pursuit : public Seek
{
public:
	virtual SteeringOutput CalculateSteering(float DeltaT, ASteeringAgent & Agent) override;
	
private:
	float DebugRadiusTarget{ 15.f };
};

class Evade : public Flee
{
	public:
	 virtual SteeringOutput CalculateSteering(float DeltaT, ASteeringAgent & Agent) override;
	
private:
	float DebugRadiusTarget{ 15.f };
};

class Wander : public Seek
{
public:
	Wander();
	virtual SteeringOutput CalculateSteering(float DeltaT, ASteeringAgent & Agent) override;
	
private:
	float Offset;
	float Radius;
	float CalculatedAngle;
	float DebugRadiusTarget{ 15.f };
};