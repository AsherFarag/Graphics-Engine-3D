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

class InspectorContext
{
public:

	void SetStartCollapsed(bool a_Val)
	{

	}
};

struct Inspector
{
	void DrawProperty(const char* Format, int& Property)
	{
		ImGui::Text(Format, Property);
	}

	void DrawProperty(const char* Format, float& Property)
	{
		ImGui::Text(Format, Property);
	}

	template < typename T >
	void DrawProperty(const char* Name, std::vector< T >& Property)
	{
		// Draw a property for size
		DrawProperty("Size", Property.size());

		int Index = 0;

		for (auto& Prop : Property)
		{
			std::string CurrentIndex = std::to_string(Index++);
			std::string ElementName = ("Element " + CurrentIndex).c_str();

			DrawProperty(ElementName.c_str(), Prop, Num);
		}
	}

	void DrawProperty(InspectorContext& Context, const char* Name, UMeshRenderer& Property)
	{

	}

	#pragma region Actors

	void DrawProperty(InspectorContext& Context, const char* Name, AActor& Property)
	{

	}

};

#endif