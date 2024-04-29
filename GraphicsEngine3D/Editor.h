#pragma once

#include <string>
#include <map>
#include "imgui.h"

namespace Editor
{
	enum EEditorStyle
	{
		ES_DEFAULT = 0,
		ES_LIGHT,
		ES_MIDNIGHT,
		NUM_OF_STYLES
	};

	static std::map< Editor::EEditorStyle, std::string > s_EditorStyles
	{
		{ ES_DEFAULT,  "Default" },
		{ ES_LIGHT,	   "Light" },
		{ ES_MIDNIGHT, "Midnight" }
	};

	extern void SetEditorStyle( Editor::EEditorStyle Style );
}

class EngineEditor
{
public:
};

