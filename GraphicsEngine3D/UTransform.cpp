#include "UTransform.h"

#pragma region Constructors

UTransform::UTransform(vec3 a_Position) 
	: m_Position(a_Position) {}

UTransform::UTransform(quat a_Rotation) 
	: m_Rotation(a_Rotation) {}

UTransform::UTransform(float a_Scale) 
	: m_Scale(a_Scale) {}

UTransform::UTransform(vec3 a_Position, quat a_Rotation) 
	: m_Position(a_Position), m_Rotation(a_Rotation) {}

UTransform::UTransform(vec3 a_Position, quat a_Rotation, vec3 a_Scale) 
	: m_Position(a_Position), m_Rotation(a_Rotation), m_Scale(a_Scale) {}

UTransform::UTransform(vec3 a_Position, quat a_Rotation, float a_Scale)
	: m_Position(a_Position), m_Rotation(a_Rotation), m_Scale(a_Scale) {}

#pragma endregion

vec3& UTransform::GetForward()
{
	return vec3(2 * (m_Rotation.x * m_Rotation.z + m_Rotation.w * m_Rotation.y),		// X
				2 * (m_Rotation.y * m_Rotation.z - m_Rotation.w * m_Rotation.x),		// Y
				1 - 2 * (m_Rotation.x * m_Rotation.x + m_Rotation.y * m_Rotation.y));	// Z
}

void UTransform::SetTransform(const mat4& a_Transform)
{
	m_Dirty = false;
	m_Transform = a_Transform;
	// Position
	m_Position  = m_Transform[3]; // 4th column of the transform matrix
	// Scale
	m_Scale.x = glm::length(vec3(m_Transform[0])); // Basis vector X
	m_Scale.y = glm::length(vec3(m_Transform[1])); // Basis vector Y
	m_Scale.z = glm::length(vec3(m_Transform[2])); // Basis vector Z
}

void UTransform::SetPosition(const vec3& a_Position)
{
	m_Dirty = true;
	m_Position = a_Position;
	Update();
}

void UTransform::SetRotation(const quat& a_Rotation)
{
	m_Dirty = true;
	m_Rotation = a_Rotation;
	Update();
}

void UTransform::SetRotation(const vec3& a_Rotation)
{
	m_Dirty = true;
	m_Rotation = glm::quat(a_Rotation);
	Update();
}

void UTransform::SetScale(const vec3& a_Scale)
{
	m_Dirty = true;
	m_Scale = a_Scale;
	Update();
}

void UTransform::Update()
{
	// Create a translation matrix
	mat4 translationMatrix = glm::translate(mat4(1.0f), m_Position);

	// Convert quaternion rotation to rotation matrix
	mat4 rotationMatrix = glm::toMat4(m_Rotation);

	// Create a scale matrix with identity matrix
	mat4 scaleMatrix = glm::scale(mat4(1.0f), m_Scale);

	// Combine the translation, rotation, and scale matrices
	m_Transform = translationMatrix * rotationMatrix * scaleMatrix;
}

#if IS_EDITOR
void UTransform::Draw_ImGui()
{
	if ( ImGui::TreeNode( "Transform" ) )
	{
		vec3 Position = m_Position;
		if ( ImGui::DragFloatXYZ( false, "Position", &Position[ 0 ], 0.01f ) )
			SetPosition( Position );

		vec3 Rotation = GetRotationEular();
		if ( ImGui::DragFloatXYZ( false, "Rotation", &Rotation[ 0 ], 0.1f ) )
			SetRotation( Rotation );

		vec3 Scale = m_Scale;
		if ( ImGui::DragFloatXYZ( false, "Scale", &Scale[ 0 ], 0.01f ) )
			SetScale( Scale );

		ImGui::TreePop();
	}
}
#endif // IS_EDITOR


