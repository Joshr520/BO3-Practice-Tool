#pragma once

#include "spdlog/spdlog.h"

#include <chrono>

typedef unsigned int ImU32;

namespace BO3PracticeTool
{
	struct ImGuiLog {
		std::string Message;
		ImU32 Color;
		spdlog::level::level_enum Level;
		std::chrono::time_point<std::chrono::system_clock> TimePoint;

		bool operator<(const ImGuiLog& log) const
		{
			if (TimePoint == log.TimePoint) {
				return Level < log.Level;
			}
			return TimePoint < log.TimePoint;
		}
	};

	typedef enum FilterLevels : int {
		Debug = 1, Info, Warning, Error
	} FilterLevel;

	class ImGuiLogs {
	public:
		static void RenderLogWindow();
		static void AddLog(spdlog::level::level_enum level, std::string_view message);
		static void FilterLogs();

		static void SetShowLogs(bool value) { s_ShowLogs = value; }
		static bool GetShowLogs() { return s_ShowLogs; }

		static FilterLevel GetFilterByName(std::string_view name);

		static std::vector<ImGuiLog>& GetLogVector(spdlog::level::level_enum level);
	private:
		static bool s_ShowLogs;
		static FilterLevel s_Filter;

		static std::vector<ImGuiLog> s_TraceLogs;
		static std::vector<ImGuiLog> s_DebugLogs;
		static std::vector<ImGuiLog> s_InfoLogs;
		static std::vector<ImGuiLog> s_WarnLogs;
		static std::vector<ImGuiLog> s_ErrorLogs;
		static std::vector<ImGuiLog> s_CriticalLogs;
		static std::vector<ImGuiLog> s_DisplayLogs;
	};
}