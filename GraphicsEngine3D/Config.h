#pragma once

#ifndef IS_EDITOR
	#define IS_EDITOR 0
#endif

#ifndef IS_BUILD
	#define IS_BUILD 0
#endif

#if IS_EDITOR

	#define INSPECTABLE friend struct Inspector;

#else

	#define LOG( Level, Time, Message )
	#define INSPECTABLE

#endif // IS_EDITOR

//
// --- Editor Includes ---
//
#if IS_EDITOR

#include "Editor.h"
#include "ImGui_DebugLog.h"
#include "ImGui_Inspector.h"
#include "ImGui_ResourceManager.h"
#include "ImGui_Viewport.h"
#include "ImGui_Window.h"

#endif

//
//  --- Includes
//
#include <vector>




