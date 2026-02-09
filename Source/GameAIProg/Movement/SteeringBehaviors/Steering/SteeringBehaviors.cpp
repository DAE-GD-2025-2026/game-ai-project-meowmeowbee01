#include "SteeringBehaviors.h"
#include "GameAIProg/Movement/SteeringBehaviors/SteeringAgent.h"

//SEEK
SteeringOutput Seek::CalculateSteering(float DeltaT, ASteeringAgent& Agent)
{
	SteeringOutput Steering {};
	Steering.LinearVelocity = Target.Position - Agent.GetPosition();
	//todo: add debug rendering
	return Steering;
}