#include "Tile.h"

Tile::Tile(vec2 a_Pos, vec2 a_HalfExtents, vec4 a_Colour, RigidBody::ECollisionType a_CollisionType)
	: Transform(a_Pos)
{
	Init(a_HalfExtents, a_Colour, a_CollisionType);
}

void Tile::Init(vec2 a_HalfExtents, vec4 a_Colour, RigidBody::ECollisionType a_CollisionType)
{
	m_BoxRB = AddComponent<BoxRB>(this, a_CollisionType, a_HalfExtents, a_Colour);
}
