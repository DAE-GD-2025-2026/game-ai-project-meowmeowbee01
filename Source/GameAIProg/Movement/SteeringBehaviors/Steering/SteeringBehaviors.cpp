#include "SteeringBehaviors.h"
#include "GameAIProg/Movement/SteeringBehaviors/SteeringAgent.h"

//Seek
SteeringOutput Seek::CalculateSteering(float DeltaT, ASteeringAgent& Agent)
{
	SteeringOutput Steering {};
	Steering.LinearVelocity = Target.Position - Agent.GetPosition();
	
	Agent.SetMaxLinearSpeed(MaxSpeed);
	
	DrawDebugLine(
		Agent.GetWorld(),
		FVector {Agent.GetPosition().X, Agent.GetPosition().Y,0},
		FVector {Target.Position.X, Target.Position.Y,0},
		FColor::Green
		);
	
	return Steering;
}

//Flee
SteeringOutput Flee::CalculateSteering(float DeltaT, ASteeringAgent& Agent)
{
	SteeringOutput Steering {};
	Steering.LinearVelocity = Agent.GetPosition() - Target.Position;
	
	Agent.SetMaxLinearSpeed(MaxSpeed);
	
	DrawDebugLine(
		Agent.GetWorld(),
		FVector {Agent.GetPosition().X, Agent.GetPosition().Y,0},
		FVector {Target.Position.X, Target.Position.Y,0},
		FColor::Red
		);
	
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
	
	DrawDebugLine(
		Agent.GetWorld(),
		FVector {Agent.GetPosition().X, Agent.GetPosition().Y,0},
		FVector {Target.Position.X, Target.Position.Y,0},
		FColor::Green
		);
	DrawDebugCircle(
		Agent.GetWorld(),
		FVector {Agent.GetPosition().X, Agent.GetPosition().Y,0},
		TargetRadius,
		20,
		FColor::Red,
		false,
		-1.f,
		0,
		0,
		FVector {0,1,0},
		FVector {1,0,0},
		false);
	DrawDebugCircle(
		Agent.GetWorld(),
		FVector {Agent.GetPosition().X, Agent.GetPosition().Y,0},
		SlowRadius,
		20,
		FColor::Blue,
		false,
		-1.f,
		0,
		0,
		FVector {0,1,0},
		FVector {1,0,0},
		false);
	
	
	
	return Steering;
}