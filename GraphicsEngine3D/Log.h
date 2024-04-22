#pragma once

//#include <chrono>
#include "GraphicsEngine3DApp.h"

#define PRINT_TODO_MESSAGES 1
enum TODO_PRIORITY_LEVEL
{
	LOW,
	MEDIUM,
	HIGH
};

#if IS_EDITOR

#define DEBUG_TIME_DECLARE std::chrono::time_point<std::chrono::high_resolution_clock> DEBUG_TIME_DECLARE_begin
#define DEBUG_TIME_BEGIN DEBUG_TIME_DECLARE_begin = std::chrono::high_resolution_clock::now()
#define DEBUG_TIME_END(Unit) std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - DEBUG_TIME_DECLARE_begin).count() * Unit

#else

#define DEBUG_TIME_DECLARE
#define DEBUG_TIME_BEGIN 
#define DEBUG_TIME_END(Unit)	

#endif // IS_EDITOR


#if PRINT_TODO_MESSAGES

#define Stringize( L )     #L 
#define WRAP( M, L ) M(L)
#define $LINE WRAP( Stringize, __LINE__ )
#define PRAGMA(p) _Pragma(#p)

#define TODO(Message)	PRAGMA(message(__FILE__ "(" $LINE ") : TODO: " Message))
#define NOT_IMPLEMENTED	PRAGMA(message(__FILE__ "(" $LINE ") : Not Implemented"))

#else

#define TODO(Message)
#define NOT_IMPLEMENTED

#endif // PRINT_TODO_MESSAGES

#define LOG( Level, Message ) GraphicsEngine3DApp::GetInstance()->m_DebugLog.PrintMessage(Debug::DebugMessage(true, Message, Debug::Level ))
#define LOG_NO_TIME( Level, Message ) GraphicsEngine3DApp::GetInstance()->m_DebugLog.PrintMessage(Debug::DebugMessage(false, Message, Debug::Level ))