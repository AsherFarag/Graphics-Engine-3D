#include "RigidBody.h"
#include "World.h"

RigidBody::RigidBody(Transform* a_Owner, EShapeType a_ShapeID, ECollisionType a_CollisionType, float a_Mass, float a_Elasticity, vec2 a_Velocity, float a_AngularVelocity, vec4 a_Colour)
	: PhysicsObject(a_Owner, a_Colour)
{
	World::GetPhysicsManager()->AddRigidBody(this);

	m_ShapeID = a_ShapeID;
	m_CollisionType = a_CollisionType;
	SetMass(a_Mass);
	SetElasticity(a_Elasticity);
	SetVelocity(a_Velocity);
	SetAngularVelocity(a_AngularVelocity);
	m_LastPosition = GetPosition();
	m_LastRotation = GetRotation();
	SetLinearDrag(DEFAULT_LINEAR_DRAG);
	SetAngularDrag(DEFAULT_ANGULAR_DRAG);
}

RigidBody::~RigidBody()
{
	World::GetPhysicsManager()->RemoveRigidBody(this);
}

void RigidBody::Update(float DeltaTime)
{
	switch (m_CollisionType)
	{
	case STATIC:
	{
		m_Velocity = vec2();
		m_AngularVelocity = 0.f;
		return;
	}
		break;
	case DYNAMIC:
	{
		CalculateAxis();
		ApplyForce(m_PhysicsManager->GetGravity() * m_Mass * DeltaTime);

		m_OwnerTransform->m_Pos += m_Velocity * DeltaTime;


		m_OwnerTransform->m_Rot += m_AngularVelocity * DeltaTime;

		m_Velocity -= m_Velocity * m_LinearDrag * DeltaTime;
		m_AngularVelocity -= m_AngularVelocity * m_AngularDrag * DeltaTime;

		//if (length(m_Velocity) < MIN_LINEAR_THRESHOLD) {
		//	m_Velocity = vec2();
		//}
		//if (abs(m_AngularVelocity) < MIN_ANGULAR_THRESHOLD) {
		//	m_AngularVelocity = 0;
		//}
		return;
	}
		break;
	case TRIGGER:
		for (auto it = m_ObjectsInside.begin(); it != m_ObjectsInside.end(); it++)
		{
			if (std::find(m_ObjectsInsideThisFrame.begin(), m_ObjectsInsideThisFrame.end(), *it) == m_ObjectsInsideThisFrame.end())
			{
				if (TriggerExitCallBack)
					TriggerExitCallBack(*it);
				it = m_ObjectsInside.erase(it);
				if (it == m_ObjectsInside.end())
					break;
			}
		}

		m_ObjectsInsideThisFrame.clear();
		break;
	default:
		break;
	}
}

void RigidBody::CalculateSmoothedPosition(float a_Alpha)
{
	m_SmoothedPosition = a_Alpha * GetPosition() + (1 - a_Alpha) * m_LastPosition;

	float SmoothedRotation = a_Alpha * GetRotation() + (1 - a_Alpha) * m_LastRotation;

	float SN = sinf(SmoothedRotation);
	float CS = cosf(SmoothedRotation);
	m_SmoothedLocalX = vec2(CS, SN);
	m_SmoothedLocalY = vec2(-SN, CS);
}

void RigidBody::ApplyForce(vec2 a_Force, vec2 a_ImpulsePosition)
{
	if (m_CollisionType == DYNAMIC)
	{
		m_Velocity += a_Force / GetMass();
		m_AngularVelocity += (a_Force.y * a_ImpulsePosition.x - a_Force.x * a_ImpulsePosition.y) / GetMoment();
	}
}

void RigidBody::ResolveCollision(RigidBody* a_Actor, vec2 a_Contact, vec2* a_CollisionNormal, float a_Penetration)
{
	m_ObjectsInsideThisFrame.push_back(a_Actor);
	a_Actor->m_ObjectsInsideThisFrame.push_back(this);

	if (IsTrigger() || a_Actor->IsTrigger())
	{
		TriggerEnter(a_Actor);
		a_Actor->TriggerEnter(this);
		return;
	}

	const vec2 Normal = normalize(a_Actor->GetPosition() - GetPosition());
	const vec2 Perpendicular(Normal.y, -Normal.x);

	// The radius frm axis to application of force
	float Radius1 = dot(a_Contact - GetPosition(), -Perpendicular);
	float Radius2 = dot(a_Contact - a_Actor->GetPosition(), Perpendicular);

	// velocity of the contact point on this object
	float Velocity1 = dot(m_Velocity, Normal) - Radius1 * m_AngularVelocity;
	// velocity of contact point on actor2
	float Velocity2 = dot(a_Actor->GetVelocity(), Normal) + Radius2 * a_Actor->GetAngularVelocity();

	if (Velocity1 > Velocity2)
	{
		float Mass1 = 1.f / (1.f / GetMass() + (Radius1 * Radius1) / GetMoment());
		float Mass2 = 1.f / (1.f / a_Actor->GetMass() + (Radius2 * Radius2) / a_Actor->GetMoment());

		float AverageElasticity = (GetElasticity() + a_Actor->GetElasticity()) / 2.f;
		vec2 ResultForce = (1.f + AverageElasticity) * Mass1 * Mass2 / (Mass1 + Mass2) * (Velocity1 - Velocity2) * Normal;
		ApplyForce(-ResultForce, a_Contact - GetPosition());
		a_Actor->ApplyForce(ResultForce, a_Contact - a_Actor->GetPosition());

		#pragma region Friction Calculation

		vec2 AngularLinearVelocityA = -Perpendicular * m_AngularVelocity;
		vec2 AngularLinearVelocityB = Perpendicular * a_Actor->GetAngularVelocity();

		float RelativeVelocityMagnitude = Velocity1 - Velocity2;
		vec2 RelativeVelocity = (a_Actor->GetVelocity() + AngularLinearVelocityB) - (GetVelocity() + AngularLinearVelocityA);

		// Calculate the friction force magnitude using the coefficient of friction
		float FrictionForceMagnitude = -m_FrictionCoefficient * RelativeVelocityMagnitude;

		// Calculate the maximum friction force magnitude based on the normal force
		float MaxFrictionForceMagnitude = m_StaticFrictionCoefficient * ResultForce.length();

		// Clamp the friction force magnitude to prevent it from exceeding the maximum friction force
		//FrictionForceMagnitude = clamp(FrictionForceMagnitude, -MaxFrictionForceMagnitude, MaxFrictionForceMagnitude);

		// Calculate the friction force vector
		vec2 FrictionForce = FrictionForceMagnitude * normalize(RelativeVelocity);

		// Apply the friction force to both objects
		ApplyForce(-FrictionForce, a_Contact - GetPosition());
		a_Actor->ApplyForce(FrictionForce, a_Contact - a_Actor->GetPosition());

		//// Calculate the relative velocity of the contact points on both objects
		//vec2 RelativeVelocity = m_Velocity - a_Actor->GetVelocity();
		//float RelativeVelocityMagnitude = Velocity1 - Velocity2;

		//// Calculate the friction force magnitude using the coefficient of friction
		//float FrictionForceMagnitude = RelativeVelocityMagnitude * -m_FrictionCoefficient;

		//// Calculate the maximum friction force magnitude based on the normal force
		//float MaxFrictionForceMagnitude = m_StaticFrictionCoefficient * ResultForce.length();

		//// Clamp the friction force magnitude to prevent it from exceeding the maximum friction force
		//FrictionForceMagnitude = clamp(FrictionForceMagnitude, -MaxFrictionForceMagnitude, MaxFrictionForceMagnitude);

		//// Calculate the friction force vector
		//vec2 FrictionForce = RelativeVelocity * -m_FrictionCoefficient;

		//// Apply the friction force to both objects
		//ApplyForce(FrictionForce, a_Contact - GetPosition());
		//a_Actor->ApplyForce(-FrictionForce, a_Contact - a_Actor->GetPosition());

		#pragma endregion

		if (CollisionCallBack != nullptr)
			CollisionCallBack(a_Actor);
		if (a_Actor->CollisionCallBack)
			a_Actor->CollisionCallBack(this);

		if (a_Penetration > 0)
			World::GetPhysicsManager()->ApplyContactForces(this, a_Actor, Normal, a_Penetration);
	}
}

void RigidBody::SetDebugDraw(bool a_Draw)
{
	//if (m_DebugDraw == a_Draw)
	//	return;

	//if (a_Draw)
	//{
	//	m_PhysicsManager->AddDebugDrawCall(Draw);
	//}
}

void RigidBody::TriggerEnter(RigidBody* a_RigidBody)
{
	if (IsTrigger() && std::find(m_ObjectsInside.begin(), m_ObjectsInside.end(), a_RigidBody) == m_ObjectsInside.end())
	{
		m_ObjectsInside.push_back(a_RigidBody);
		if (TriggerEnterCallBack != nullptr)
			TriggerEnterCallBack(a_RigidBody);
	}
}

void RigidBody::TriggerExit(RigidBody* a_RigidBody)
{
}
