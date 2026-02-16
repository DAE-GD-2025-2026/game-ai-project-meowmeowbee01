
#include "CombinedSteeringBehaviors.h"
#include <algorithm>
#include "../SteeringAgent.h"

BlendedSteering::BlendedSteering(const std::vector<WeightedBehavior>& WeightedBehaviors)
	:WeightedBehaviors(WeightedBehaviors)
{};

//****************
//BLENDED STEERING
SteeringOutput BlendedSteering::CalculateSteering(float DeltaT, ASteeringAgent& Agent)
{
	const float DebugDrawingScale {250.f};
	SteeringOutput BlendedSteering = {};
	for (const WeightedBehavior& Behavior : WeightedBehaviors)
	{
		auto Output {Behavior.pBehavior->CalculateSteering(DeltaT, Agent)};
		Output.LinearVelocity.Normalize();
		BlendedSteering.LinearVelocity += Output.LinearVelocity * Behavior.Weight;
		
		DrawDebugLine(
			Agent.GetWorld(),
			FVector {Agent.GetPosition().X, Agent.GetPosition().Y,0},
			FVector
			{
				Agent.GetPosition().X + Output.LinearVelocity.X * Behavior.Weight * DebugDrawingScale, 
				Agent.GetPosition().Y + Output.LinearVelocity.Y * Behavior.Weight * DebugDrawingScale,
				0
			},
			FColor::Magenta);
	}
	
	DrawDebugLine(
	Agent.GetWorld(),
	FVector {Agent.GetPosition().X, Agent.GetPosition().Y,0},
	FVector
	{
		Agent.GetPosition().X + BlendedSteering.LinearVelocity.X * DebugDrawingScale, 
		Agent.GetPosition().Y + BlendedSteering.LinearVelocity.Y * DebugDrawingScale,
		0
	},
	FColor::Cyan);

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

	//If none of the behavior return a valid output, last behavior is returned
	return Steering;
}