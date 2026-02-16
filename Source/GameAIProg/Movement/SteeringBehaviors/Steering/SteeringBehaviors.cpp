#include "SteeringBehaviors.h"

#include "VectorTypes.h"
#include "GameAIProg/Movement/SteeringBehaviors/SteeringAgent.h"

//Seek
SteeringOutput Seek::CalculateSteering(float DeltaT, ASteeringAgent& Agent)
{
	Agent.SetMaxLinearSpeed(MaxSpeed);
	
	SteeringOutput Steering {};
	Steering.LinearVelocity = Target.Position - Agent.GetPosition();
	
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
	Agent.SetMaxLinearSpeed(MaxSpeed);
	
	SteeringOutput Steering {};
	Steering.LinearVelocity = Agent.GetPosition() - Target.Position;
	
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

//Face
SteeringOutput Face::CalculateSteering(float DeltaT, ASteeringAgent& Agent)
{
	Agent.SetMaxLinearSpeed(0);
	
	SteeringOutput Steering {};
	Steering.LinearVelocity = Target.Position - Agent.GetPosition();
	
	DrawDebugLine(
		Agent.GetWorld(),
		FVector {Agent.GetPosition().X, Agent.GetPosition().Y,0},
		FVector {Target.Position.X, Target.Position.Y,0},
		FColor::Green);
	
	return Steering;
}

//Pursuit
SteeringOutput Pursuit::CalculateSteering(float DeltaT, ASteeringAgent& Agent)
{
	Agent.SetMaxLinearSpeed(MaxSpeed);
	
	SteeringOutput Steering {};
	const FVector2D Intercept {Target.Position + Target.LinearVelocity * TimeToIntercept};
	TimeToIntercept = UE::Geometry::Distance(Intercept, Agent.GetPosition()) / Agent.GetMaxLinearSpeed();
	Steering.LinearVelocity = Intercept - Agent.GetPosition();
	
	DrawDebugLine(
		Agent.GetWorld(),
		FVector {Agent.GetPosition().X, Agent.GetPosition().Y,0},
		FVector {Intercept.X, Intercept.Y,0},
		FColor::Green);
	
	return Steering;
}

//Evade
SteeringOutput Evade::CalculateSteering(float DeltaT, ASteeringAgent& Agent)
{
	SteeringOutput Steering {};
	const FVector2D Intercept {Target.Position + Target.LinearVelocity * TimeToIntercept};
	TimeToIntercept = UE::Geometry::Distance(Intercept, Agent.GetPosition()) / Agent.GetMaxLinearSpeed();
	Steering.LinearVelocity = Agent.GetPosition() - Intercept;
	
	Agent.SetMaxLinearSpeed(MaxSpeed);
	
	DrawDebugLine(
		Agent.GetWorld(),
		FVector {Agent.GetPosition().X, Agent.GetPosition().Y,0},
		FVector {Intercept.X, Intercept.Y,0},
		FColor::Red);
	
	return Steering;
}

//Wander
SteeringOutput Wander::CalculateSteering(float DeltaT, ASteeringAgent& Agent)
{
	Agent.SetMaxLinearSpeed(MaxSpeed);
	
	SteeringOutput Steering {};
	m_WanderAngle += FMath::RandRange(-m_MaxAngleChange, m_MaxAngleChange);
	FVector2D AgentVelocity {Agent.GetLinearVelocity()};
	AgentVelocity.Normalize();
	const FVector2D CircleCenter {Agent.GetPosition() + AgentVelocity * m_OffsetDistance};
	const FVector2D WanderTarget {CircleCenter.X + cosf(m_WanderAngle) * m_Radius, CircleCenter.Y - sinf(m_WanderAngle) * m_Radius};
	Steering.LinearVelocity = WanderTarget - Agent.GetPosition();
	
	DrawDebugLine(
		Agent.GetWorld(),
		FVector {Agent.GetPosition().X, Agent.GetPosition().Y,0},
		FVector {WanderTarget.X, WanderTarget.Y,0},
		FColor::Green);
	DrawDebugCircle(
		Agent.GetWorld(),
		FVector {CircleCenter.X, CircleCenter.Y,0},
		m_Radius,
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