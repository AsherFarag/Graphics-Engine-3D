#pragma once

#include "imgui.h"

enum EEditorStyle
{
	ES_MIDNIGHT,
};

class Editor
{
public:
	static void SetEditorStyle(EEditorStyle Style);
};

