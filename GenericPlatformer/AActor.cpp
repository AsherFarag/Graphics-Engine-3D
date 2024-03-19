#include "AActor.h"

#include "World.h"

AActor::AActor(vec2 a_Pos, float a_Rot)
	: Transform(a_Pos, a_Rot)
{
	World::AddActor(this);
}

AActor::~AActor()
{
	World::RemoveActor(this);
}
