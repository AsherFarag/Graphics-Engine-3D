#pragma once
#include "ImGui_Window.h"

// --- STD ---
#include <vector>
#include <iostream>
#include <chrono>

namespace Debug
{
	enum ELogLevel
	{
		Default,
		Warning,
		Error
	};

	struct DebugMessage
	{
		DebugMessage(bool a_PrintTime = true, const char* a_Message = "Hello World!", ELogLevel a_LogLevel = ELogLevel::Default, ImVec4 a_Color = ImVec4(1, 1, 1, 1))
		{
			if (a_PrintTime)
			{
				// Get the current system time
				const auto Now = std::chrono::system_clock::now();

				// Convert the system time to seconds since epoch
				auto Now_Seconds = std::chrono::time_point_cast<std::chrono::seconds>(Now);

				// Get the time since epoch in seconds
				auto Epoch_Seconds = Now_Seconds.time_since_epoch().count();

				// Calculate hours, minutes, and seconds
				auto Hours	 = (Epoch_Seconds / 3600) % 24 + 11;
				auto Minutes = (Epoch_Seconds % 3600) / 60;
				auto Seconds = Epoch_Seconds % 60;

				// Format as "H:M:S - "
				m_Message = std::to_string(Hours) + ":" + std::to_string(Minutes) + ":" + std::to_string(Seconds) + " - ";
			}

			m_LogLevel = a_LogLevel;
			switch (m_LogLevel)
			{
			case ELogLevel::Default:
				m_Message.append(a_Message);
				m_Color = a_Color;
				break;
			case ELogLevel::Warning:
				m_Message += std::string("Warning: ").append(a_Message);
				m_Color = ImVec4(1, 1, 0, 1);
				break;
			case ELogLevel::Error:
				m_Message += std::string("Error: ").append(a_Message);
				m_Color = ImVec4(1, 0, 0, 1);
				break;
			}

			m_Message.append("\n");
		}

		~DebugMessage()
		{
		}

		ELogLevel	m_LogLevel;
		std::string	m_Message;
		ImVec4		m_Color;
	};

	class ImGui_DebugLog
		: public ImGui_Window
	{
	public:
		ImGui_DebugLog();
		virtual ~ImGui_DebugLog();

		void PrintMessage(DebugMessage a_DebugMessage);

	protected:

		std::vector<DebugMessage> m_DebugMessages;

	protected:
		virtual void BeginDraw();
		virtual void OnDraw();
		virtual void EndDraw();
	};
}

