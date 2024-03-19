#include "UBaseComponent.h"

// --- Engine ---
#include "AActor.h"

UBaseComponent::UBaseComponent(AActor* a_Owner)
{
	if (a_Owner)
		SetOwner(a_Owner);
}

UBaseComponent::~UBaseComponent()
{

}

void UBaseComponent::SetOwner(AActor* a_Owner)
{
	m_Owner = a_Owner;
}
