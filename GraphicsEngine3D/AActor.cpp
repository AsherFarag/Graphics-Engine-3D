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
}

AActor::~AActor()
{
}

void AActor::Update()
{
	UpdateTransform();
}

void AActor::UpdateTransform(bool a_Force)
{
	if (m_Dirty || a_Force)
	{
		m_Transform = mat4(1);
		m_Transform = translate(m_Transform, m_Position);
		m_Transform = rotate(m_Transform, radians(m_Rotation.x), vec3(1.f, 0.f, 0.f));
		m_Transform = rotate(m_Transform, radians(m_Rotation.y), vec3(0.f, 1.f, 0.f));
		m_Transform = rotate(m_Transform, radians(m_Rotation.z), vec3(0.f, 0.f, 1.f));
		m_Transform = scale(m_Transform, m_Scale);
	}
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

	#pragma region Transform

	if (ImGui::DragFloatXYZ(false, "Position", &m_Position[0], 0.01f))
		SetPosition(m_Position);

	static float Rotation[] = { 0.0, 0.0, 0.0 };
	if (ImGui::DragFloatXYZ(false, "Rotation", &m_Rotation[0], 0.1f))
		SetRotation(m_Rotation);

	static float Scale[] = { 1, 1, 1 };
	if (ImGui::DragFloatXYZ(false, "Scale", &m_Scale[0], 0.01f))
		SetScale(m_Scale);

	#pragma endregion

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

void AActor::OnDraw_ImGui()
{
}