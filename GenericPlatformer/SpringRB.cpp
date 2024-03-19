#include "SpringRB.h"

//SpringRB::SpringRB()
//{
//}

SpringRB::SpringRB(Transform* a_Owner, RigidBody* a_Body1, RigidBody* a_Body2, float a_SpringCoefficient, float a_Damping, vec4 a_Colour, float a_RestLength, vec2 a_Contact1, vec2 a_Contact2)
	: PhysicsObject(a_Owner, a_Colour)
{
	m_Body1				= a_Body1;
	m_Body2				= a_Body2;
	m_SpringCoefficient = a_SpringCoefficient;
	m_Damping			= a_Damping;
	m_RestLength		= a_RestLength;
	m_Contact1			= a_Contact1;
	m_Contact2			= a_Contact2;

	
	if (a_RestLength == 0.f)
	{
		if (m_Body1)
		{
			m_Body1->CalculateAxis();
			m_Body1->SetDynamicFrictionCoefficient(0);
			m_Body1->SetStaticFrictionCoefficient(0);
		}
		if (m_Body2)
		{
			m_Body2->CalculateAxis();
			m_Body2->SetDynamicFrictionCoefficient(0);
			m_Body2->SetStaticFrictionCoefficient(0);
		}
		m_RestLength = distance(GetContact1(), GetContact2());
	}
}

void SpringRB::Update(float DeltaTime)
{
	vec2 P1 = GetContact1();
	vec2 P2 = GetContact2();

	float Length = distance(P1, P2);
	vec2 Direction = normalize(P2 - P1);

	// Apply Damping
	vec2 RelativeVelocity = m_Body2->GetVelocity() - m_Body1->GetVelocity();

	// F = -kX - bv
	vec2 Force = Direction * m_SpringCoefficient * (m_RestLength - Length) - m_Damping * RelativeVelocity;
	const float Threshold = 1000.f;
	float ForceMagnitude = length(Force);
	if (ForceMagnitude > Threshold)
		Force *= Threshold / ForceMagnitude;

	if (m_RestLength > 0.f)
	{
		m_Body1->ApplyForce(-Force * DeltaTime, P1 - m_Body1->GetPosition());
		m_Body2->ApplyForce(Force * DeltaTime, P2 - m_Body2->GetPosition());
	}
}

void SpringRB::Draw(float a_Alpha)
{
	aie::Gizmos::add2DLine(GetContact1(a_Alpha), GetContact2(a_Alpha), m_Colour);
}
