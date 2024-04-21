#include "AActor.h"

// --- Engine ---
#include "World.h"
#include "MathF.h"
#include "UBaseComponent.h"

AActor::AActor()
	: UBaseObject()
{
	m_Name = "Actor";
	m_World->AddActor(this);

	m_Transform = AddComponent<UTransform>();
}

AActor::~AActor()
{
}

void AActor::Update()
{
}

vec3& AActor::GetActorPosition()
{
	return m_Transform->GetPosition();
}

quat& AActor::GetActorRotation()
{
	return m_Transform->GetRotation();
}
vec3& AActor::GetActorRotationEular()
{
	return m_Transform->GetRotationEular();
}
vec3& AActor::GetActorScale()
{
	return m_Transform->GetScale();
}

vec3 AActor::GetForward()
{
	return vec3();
}

void AActor::SetActorPosition(const vec3& a_Position)
{
	m_Transform->SetPosition(a_Position);
}

void AActor::SetActorRotation(const quat& a_Rotation)
{
	m_Transform->SetRotation(a_Rotation);
}

void AActor::SetActorScale(const vec3& a_Scale)
{
	m_Transform->SetScale(a_Scale);
}

void AActor::SetActive(bool NewActive, bool PropagateToChildren, bool PropagateToComponents)
{
	SetEnabled(NewActive);

	if (PropagateToChildren)
	{
		// TODO: Implement Children
	}

	if (PropagateToComponents)
	{
		for (auto C : m_Components)
		{
			C.second->SetEnabled(NewActive);
		}
	}
}

void AActor::OnEnabled()
{
}

void AActor::OnDisabled()
{
}

void AActor::Draw_ImGui()
{
	// - Enabled -
	static bool Enabled = IsEnabled();
	if (ImGui::Checkbox("Enabled", &Enabled))
		SetActive(Enabled);

	ImGui::SameLine();
	// - Name -
	char Name[256];
	m_Name.copy(Name, m_Name.length());
	Name[m_Name.length()] = 0;
	if (ImGui::InputText("Name", Name, 256))
		SetName(Name);

	if (ImGui::TreeNode("Info"))
	{
		ImGui::Text("ID: [%i]", GetId());

		ImGui::TreePop();
	}

	//#pragma region Transform

	//if (ImGui::DragFloatXYZ(false, "Position", &m_Position[0], 0.01f))
	//	SetActorPosition(m_Position);

	//static float Rotation[] = { 0.0, 0.0, 0.0 };
	//if (ImGui::DragFloatXYZ(false, "Rotation", &m_Rotation[0], 0.1f))
	//	SetActorRotation(m_Rotation);

	//static float Scale[] = { 1, 1, 1 };
	//if (ImGui::DragFloatXYZ(false, "Scale", &m_Scale[0], 0.01f))
	//	SetActorScale(m_Scale);

	//#pragma endregion

	OnDraw_ImGui();

	// - Components -
	if (m_Components.size() > 0)
	{
		if (ImGui::TreeNode("Components"))
		{
			for (auto C : m_Components)
			{
				C.second->Draw_ImGui();
			}
			ImGui::TreePop();
		}
	}
}

