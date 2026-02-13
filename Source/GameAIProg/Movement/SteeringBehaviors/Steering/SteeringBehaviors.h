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
	virtual SteeringOutput CalculateSteering(float DeltaT, ASteeringAgent& Agent) = 0;

	void SetTarget(const FTargetData& NewTarget) { Target = NewTarget; }

	template<class T, std::enable_if_t<std::is_base_of_v<ISteeringBehavior, T>>* = nullptr>
	T* As()
	{
		return static_cast<T*>(this);
	}

protected:
	FTargetData Target;
	const float MaxSpeed {500.f};
};

class Seek : public ISteeringBehavior
{
public:
	Seek() = default;
	virtual ~Seek() override = default;

	virtual SteeringOutput CalculateSteering(float DeltaT, ASteeringAgent& Agent) override;
};

class Flee : public ISteeringBehavior
{
public:
	Flee() = default;
	virtual ~Flee() override = default;

	virtual SteeringOutput CalculateSteering(float DeltaT, ASteeringAgent& Agent) override;
};

class Arrive : public ISteeringBehavior
{
public:
	Arrive() = default;
	virtual ~Arrive() override = default;

	const float SlowRadius {600.f};
	const float TargetRadius {300.f};

	virtual SteeringOutput CalculateSteering(float DeltaT, ASteeringAgent& Agent) override;
};

class Face : public ISteeringBehavior
{
public:
	Face() = default;
	virtual ~Face() override = default;

	virtual SteeringOutput CalculateSteering(float DeltaT, ASteeringAgent& Agent) override;
};


class Pursuit : public ISteeringBehavior
{
public:
	Pursuit() = default;
	virtual ~Pursuit() override = default;
	
	float TimeToIntercept {0.f};

	virtual SteeringOutput CalculateSteering(float DeltaT, ASteeringAgent& Agent) override;
};

class Evade : public ISteeringBehavior
{
public:
	Evade() = default;
	virtual ~Evade() override = default;
	
	float TimeToIntercept {0.f};

	virtual SteeringOutput CalculateSteering(float DeltaT, ASteeringAgent& Agent) override;
};

class Wander : public Seek
{
public:
	Wander() = default;
	virtual ~Wander() override = default;
	
	void SetWanderOffset(float offset) {m_OffsetDistance = offset;}
	void SetWanderRadius(float radius) {m_Radius = radius;}
	void SetMaxAngleChange(float rad) {m_MaxAngleChange = rad;}

protected:
	float m_OffsetDistance {200.f};
	float m_Radius {100.f};
	float m_MaxAngleChange {20 * PI / 180.f};
	
	float m_WanderAngle {};

	virtual SteeringOutput CalculateSteering(float DeltaT, ASteeringAgent& Agent) override;
};