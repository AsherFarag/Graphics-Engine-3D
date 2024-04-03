#pragma once

// --- ImGui ---
#include "imgui.h"

// --- STD ---
#include <vector>

// --- Engine ---
// Properties
#include "UBaseObject.h"
#include "AActor.h"
#include "UMeshRenderer.h"

#if IS_EDITOR

struct Inspector
{
	void DrawProperty(int& Property, const char* Format = "%i")
	{
		ImGui::Text(Format, Property);
	}

	void DrawProperty(float& Property, const char* Format = "%.3f")
	{
		ImGui::Text(Format, Property);
	}

	void DrawProperty(bool& Property, const char* Format = "%i")
	{
		ImGui::Text(Format, Property);
	}

	template < typename T >
	void DrawProperty(const char* Name, std::vector< T >& Property)
	{
		// Draw a property for size
		DrawProperty(Property.size(), "Size: %i");

		int Index = 0;
		if (ImGui::TreeNode("Components"))
		{
			for (auto& Prop : Property)
			{
				std::string CurrentIndex = std::to_string(Index++);
				std::string ElementName = ("Element %i:" + CurrentIndex).c_str();

				DrawProperty(ElementName.c_str(), Prop, Num);
			}
		}
	}

	void DrawProperty(const char* Name, UMeshRenderer& Property)
	{

	}

	#pragma region Actors

	void DrawProperty(const char* Name, AActor& Property)
	{

	}

};

#endif