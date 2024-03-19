#include "UBaseObject.h"
#include "World.h"

UBaseObject::UBaseObject()
{
	ID = rand();

	Engine = GraphicsEngine3DApp::GetInstance();
	World::GetWorld()->AddObject(this);
	m_Valid = true;
}

UBaseObject::~UBaseObject()
{
}

bool UBaseObject::Destroy(float a_LifeTime)
{
	if (m_Valid && m_World)
	{
		m_Valid = false;
		OnDestroyed();
		m_World->DestroyObject(this);

	}

	return !m_Valid;
}

bool UBaseObject::SetWorld(World* a_World)
{
	m_World = a_World;

	return m_World != nullptr;
}

void UBaseObject::SetEnabled(bool a_Enabled)
{
	// Do nothing as we are already in the desired state
	if (m_Enabled == a_Enabled)
		return;

	m_Enabled = a_Enabled;

	if (m_Enabled)
		OnEnabled();
	else
		OnDisabled();
}

World* UBaseObject::GetWorld() const
{
	return m_World;
}
