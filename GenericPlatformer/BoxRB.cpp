#include "BoxRB.h"

BoxRB::BoxRB(Transform* a_Owner, ECollisionType a_CollisionType, vec2 a_Extents, vec4 a_Colour, float a_Mass, float a_Elasticity, vec2 a_Velocity, float a_AngularVelocity)
	: RigidBody(a_Owner, BOX, a_CollisionType, a_Mass, a_Elasticity, a_Velocity, a_AngularVelocity, a_Colour)
{
	m_Extents = a_Extents;
	CalculateMoment();
}

void BoxRB::Update(float DeltaTime)
{
	CalculateAxis();
	RigidBody::Update(DeltaTime);
}

void BoxRB::Draw(float a_Alpha)
{
	CalculateSmoothedPosition(a_Alpha);
	// Draw using Local Axis
	vec2 p1 = m_SmoothedPosition - m_SmoothedLocalX * m_Extents.x
		- m_SmoothedLocalY * m_Extents.y;
	vec2 p2 = m_SmoothedPosition + m_SmoothedLocalX * m_Extents.x
		- m_SmoothedLocalY * m_Extents.y;
	vec2 p3 = m_SmoothedPosition - m_SmoothedLocalX * m_Extents.x
		+ m_SmoothedLocalY * m_Extents.y;
	vec2 p4 = m_SmoothedPosition + m_SmoothedLocalX * m_Extents.x
		+ m_SmoothedLocalY * m_Extents.y;
	aie::Gizmos::add2DTri(p1, p2, p4, m_Colour);
	aie::Gizmos::add2DTri(p1, p4, p3, m_Colour);

	// Outline
	//aie::Gizmos::add2DLine(p1, p2, vec4(1));
	//aie::Gizmos::add2DLine(p3, p1, vec4(1));
	//aie::Gizmos::add2DLine(p2, p4, vec4(1));
	//aie::Gizmos::add2DLine(p4, p3, vec4(1));
}

void BoxRB::CalculateMoment()
{
	m_Moment = 1.0f / 12.0f * m_Mass * (GetWidth() * GetWidth()) + (GetHeight() * GetHeight());;
}

bool BoxRB::IsInside(vec2 a_Point)
{
	vec2 TranslatePoint = a_Point - GetPosition();


	return abs(TranslatePoint.x) < m_LocalX.x && abs(TranslatePoint.y) < m_LocalY.y;
}

bool BoxRB::CheckBoxCorners(BoxRB * a_Box, vec2 & a_Contact, int & a_NumContacts, float & a_Penetration, vec2 & a_EdgeNormal)
{
	float minX, maxX, minY, maxY;
	float boxW = a_Box->GetExtents().x * 2;
	float boxH = a_Box->GetExtents().y * 2;
	int numLocalContacts = 0;
	vec2 localContact(0, 0);
	bool first = true;

	// loop over all corners of the other box
	for (float x = -a_Box->GetExtents().x; x < boxW; x += boxW)
	{
		for (float y = -a_Box->GetExtents().y; y < boxH; y += boxH)
		{
			// Get the position in worldspace
			vec2 p = a_Box->GetPosition() + x * a_Box->GetLocalX() + y * a_Box->GetLocalY();
			// Get the position in our box's space
			vec2 p0(dot(p - GetPosition(), GetLocalX()),
				dot(p - GetPosition(), GetLocalY()));

			// update the extents in each cardinal direction in our box's space
			// (ie extents along the separating axes)
			if (first || p0.x < minX) minX = p0.x;
			if (first || p0.x > maxX) maxX = p0.x;
			if (first || p0.y < minY) minY = p0.y;
			if (first || p0.y > maxY) maxY = p0.y;

			// if this corner is inside the box, add it to the list of contact points
			if (p0.x >= -GetExtents().x && p0.x <= GetExtents().x &&
				p0.y >= -GetExtents().y && p0.y <= GetExtents().y)
			{
				numLocalContacts++;
				localContact += p0;
			}
			first = false;
		}
	}

	// if we lie entirely to one side of the box along one axis, we've found a separating
	// axis, and we can exit
	if (maxX <= -GetExtents().x || minX >= GetExtents().x ||
		maxY <= -GetExtents().y || minY >= GetExtents().y)
		return false;
	if (numLocalContacts == 0)
		return false;

	bool Result = false;
	a_Contact += GetPosition() + (localContact.x * GetLocalX() + localContact.y * GetLocalY()) /
		(float)numLocalContacts;
	a_NumContacts++;

	// find the minimum a_Penetrationetration vector as a a_Penetrationetration amount and normal
	float Penetration0 = GetExtents().x - minX;
	if (Penetration0 > 0 && (Penetration0 < a_Penetration || a_Penetration == 0)) {
		a_EdgeNormal = GetLocalX();
		a_Penetration = Penetration0;
		Result = true;
	}
	Penetration0 = maxX + GetExtents().x;
	if (Penetration0 > 0 && (Penetration0 < a_Penetration || a_Penetration == 0)) {
		a_EdgeNormal = -GetLocalX();
		a_Penetration = Penetration0;
		Result = true;
	}
	Penetration0 = GetExtents().y - minY;
	if (Penetration0 > 0 && (Penetration0 < a_Penetration || a_Penetration == 0)) {
		a_EdgeNormal = GetLocalY();
		a_Penetration = Penetration0;
		Result = true;
	}
	Penetration0 = maxY + GetExtents().y;
	if (Penetration0 > 0 && (Penetration0 < a_Penetration || a_Penetration == 0)) {
		a_EdgeNormal = -GetLocalY();
		a_Penetration = Penetration0;
		Result = true;
	}

	return Result;
}
