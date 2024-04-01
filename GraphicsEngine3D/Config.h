#pragma once

#ifndef IS_EDITOR
	#define IS_EDITOR 0
#endif

#ifndef IS_BUILD
	#define IS_BUILD 0
#endif

#if IS_EDITOR
	#define LOG( Level, Time, Message )  World::DebugLog(Debug::DebugMessage(true, Message, Debug::Level ))
#else
	#define LOG( Level, Time, Message )
#endif
