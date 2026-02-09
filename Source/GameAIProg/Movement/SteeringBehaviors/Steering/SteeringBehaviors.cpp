#include "SteeringBehaviors.h"
#include "GameAIProg/Movement/SteeringBehaviors/SteeringAgent.h"

//Seek
SteeringOutput Seek::CalculateSteering(float DeltaT, ASteeringAgent& Agent)
{
	SteeringOutput Steering {};
	Steering.LinearVelocity = Target.Position - Agent.GetPosition();
	Agent.SetMaxLinearSpeed(MaxSpeed);
	//todo: add debug rendering
	return Steering;
}

//Flee
SteeringOutput Flee::CalculateSteering(float DeltaT, ASteeringAgent& Agent)
{
	SteeringOutput Steering {};
	Steering.LinearVelocity = Agent.GetPosition() - Target.Position;
	Agent.SetMaxLinearSpeed(MaxSpeed);
	//todo: add debug rendering
	return Steering;
}

//Arrive
SteeringOutput Arrive::CalculateSteering(float DeltaT, ASteeringAgent& Agent)
{
	SteeringOutput Steering {};
	const FVector2D Difference {Target.Position - Agent.GetPosition()};
	Steering.LinearVelocity = Difference;
	const float Distance {static_cast<float>(Difference.Length())};
	float SpeedFactor {(Distance - TargetRadius) / (SlowRadius)};
	SpeedFactor = std::clamp(SpeedFactor, 0.f, 1.f);
	Agent.SetMaxLinearSpeed(SpeedFactor * MaxSpeed);
	//todo: add debug rendering
	return Steering;
}