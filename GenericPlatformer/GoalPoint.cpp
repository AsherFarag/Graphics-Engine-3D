#include "GoalPoint.h"

#include "APlayer.h"

GoalPoint::GoalPoint(vec2 a_Pos, vec2 a_HalfExtents, vec4 a_Colour)
	: Tile(a_Pos, a_HalfExtents, a_Colour, RigidBody::TRIGGER)
{
	m_BoxRB->TriggerEnterCallBack = std::bind(&GoalPoint::OnEnter, this, std::placeholders::_1);
}

void GoalPoint::OnEnter(RigidBody* a_RB)
{
	if (dynamic_cast<APlayer*>(a_RB->GetTransform()))
	{
		World::GetWorld()->ReachedGoal();
	}
}
