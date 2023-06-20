#pragma once

#include <string>
#include <vector>
#include <fstream>

#include "imgui.h"

#define COLOR_ERROR ImVec4(1.0f, 0.0f, 0.0f, 1.0f)
#define COLOR_WARNING ImVec4(1.0f, 0.5f, 0.25f, 1.0f)
#define COLOR_INFO ImVec4(1.0f, 1.0f, 0.0f, 1.0f)
#define COLOR_SUCCESS ImVec4(0.0f, 1.0f, 0.0f, 1.0f)
#define COLOR_DEBUG ImVec4(1.0f, 0.45f, 0.9f, 1.0f)
#define COLOR_UNKNOWN ImVec4(1.0f, 1.0f, 1.0f, 1.0f)
#define LOG_WINDOW_COLLAPSED_SIZE ImVec2(40.0f, 40.0f)

namespace Walnut
{

	enum class MessageType
	{
		Error,
		Warning,
		Info,
		Success,
		Debug
	};

	struct Message
	{
		std::string message;
		ImVec4 color;
	};

	class Logger
	{
	public:
		static void InitLogger(std::string_view filename);
		static void Log(MessageType type, std::string_view message);
		static void DrawLogWindow();
		static void ToggleCollapsed();
	private:
		static std::vector<Message> logMessages;
		static std::ofstream logFile;
		static std::string logFilename;
		static bool collapsed;
		static ImVec2 logWindowSize;
		static ImVec2 logWindowFull;
	};
}