#pragma once

// --- ImGui ---
#include "imgui.h"

// --- STD ---
#include <vector>

// --- Engine ---
// Properties
#include "AActor.h"
#include "UMeshRenderer.h"
#include "UTransform.h"

#if IS_EDITOR

struct Inspector
{
	void DrawProperty( const char* Name, size_t& Property )
	{
		ImGui::Text( ( Name + std::string( ": %i" ) ).c_str(), Property );
	}

	void DrawProperty(const char* Name, int& Property)
	{
		ImGui::Text((Name + std::string(": %i")).c_str(), Property);
	}

	void DrawProperty(const char* Name, float& Property)
	{
		ImGui::Text((Name + std::string(": %.3f")).c_str(), Property);
	}
	void DrawProperty(const char* Name, double& Property)
	{
		ImGui::Text((Name + std::string(": %.6f")).c_str(), Property);
	}
	void DrawProperty(const char* Name, bool& Property)
	{
		ImGui::Checkbox((Name + std::string(": ")).c_str(), &Property);
	}

	template < typename T >
	void DrawProperty(const char* Name, std::vector< T >& Property)
	{
		// Draw a property for size
		//DrawProperty("Size", Property.size());

		int Index = 0;
		if (ImGui::TreeNode(Name))
		{
			for (auto& Element : Property)
			{
				std::string ElementName("Element %i:", Index );

				//DrawProperty(ElementName.c_str(), Element);
			}
		}
	}

	void DrawProperty(const char* Name, UMeshRenderer& Property)
	{

	}
#pragma region Actors

	void DrawProperty(const char* Name, AActor& Property)
	{
		DrawProperty(Name, Property.m_Components);
	}

#pragma endregion


#pragma region Components

	void DrawProperty(const char* Name, UTransform& Property)
	{
		//vec3 Position = Property.m_Position;
		//if (ImGui::DragFloatXYZ(false, "Position", &Position[0], 0.01f))
		//	Property.SetPosition(Position);
		//
		//vec3 Rotation = Property.GetRotationEular();
		//if (ImGui::DragFloatXYZ(false, "Rotation", &Rotation[0], 0.1f))
		//	Property.SetRotation(Rotation);
		//
		//vec3 Scale = Property.m_Scale;
		//if (ImGui::DragFloatXYZ(false, "Scale", &Scale[0], 0.01f))
		//	Property.SetScale(Scale);
	}

#pragma endregion



};

#endif