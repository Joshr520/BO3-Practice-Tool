#include "LogUI.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_internal.h"

#include "Walnut/ImGui/ImGuiTheme.h"
#include "ImGui/ImGuiExtendedWidgets.h"
#include "PracticeTool/Core/Log.h"

#include <chrono>

ImU32 GetColorByLevel(spdlog::level::level_enum level);

#define COLOR_TRACE IM_COL32(255, 255, 255, 255)
#define COLOR_DEBUG IM_COL32(255, 108, 180, 255)
#define COLOR_INFO IM_COL32(255, 255, 0, 255)
#define COLOR_WARN IM_COL32(255, 150, 0, 255)
#define COLOR_ERR IM_COL32(255, 70, 0, 255)
#define COLOR_CRITICAL IM_COL32(255, 0, 0, 255)

const std::vector<std::string> filterItems = { "Debug", "Info", "Warning", "Error" };
int filterIndex = 1;

ImGuiWindowFlags logFlags = ImGuiWindowFlags_NoCollapse;

namespace BO3PracticeTool
{
	bool ImGuiLogs::s_ShowLogs = false;
	FilterLevel ImGuiLogs::s_Filter = FilterLevel::Info;
	std::vector<ImGuiLog> ImGuiLogs::s_TraceLogs;
	std::vector<ImGuiLog> ImGuiLogs::s_DebugLogs;
	std::vector<ImGuiLog> ImGuiLogs::s_InfoLogs;
	std::vector<ImGuiLog> ImGuiLogs::s_WarnLogs;
	std::vector<ImGuiLog> ImGuiLogs::s_ErrorLogs;
	std::vector<ImGuiLog> ImGuiLogs::s_CriticalLogs;
	std::vector<ImGuiLog> ImGuiLogs::s_DisplayLogs;

	void ImGuiLogs::RenderLogWindow()
	{
		if (!s_ShowLogs) {
			return;
		}

		ImVec2 size = ImGui::GetWindowSize() * 0.75f;
		ImVec2 pos = ImGui::GetWindowPos() + (ImGui::GetWindowSize() * 0.25f) / 2.0f;

		ImGui::PushStyleColor(ImGuiCol_WindowBg, Walnut::UI::Colors::ColorWithMultipliedValue(Walnut::UI::Colors::Theme::titlebar, 1.5f));

		ImGui::SetNextWindowSize(size, ImGuiCond_Once);
		ImGui::SetNextWindowPos(pos, ImGuiCond_Once);
		if (!ImGui::Begin("Logs", &s_ShowLogs, logFlags)) {
			ImGui::PopStyleColor();
			return ImGui::End();
		}

		if (ImGui::IsWindowDocked()) {
			logFlags |= ImGuiWindowFlags_NoMove;
		}

		ImGui::SetNextItemWidth(200.0f);
		if (ImGui::BeginCombo("Select Filter Level", filterItems[filterIndex].c_str())) {
			if (ImGui::Selection::Render(filterItems, filterIndex)) {
				s_Filter = GetFilterByName(filterItems[filterIndex]);
				FilterLogs();
			}
			ImGui::EndCombo();
		}
		ImGui::SameLine();
		ImGui::HelpMarker("Selecting a filter will show every message at that filter level and the filter levels below it.");

		ImGui::SameLine();
		if (ImGui::Button("Add Logs")) {
			Log::PrintMessage(std::source_location::current(), spdlog::level::level_enum::trace, "Testing Logger: {}", "Variadic Message");
			Log::PrintMessage(std::source_location::current(), spdlog::level::level_enum::debug, "Testing Logger: {}", "Variadic Message");
			Log::PrintMessage(std::source_location::current(), spdlog::level::level_enum::info, "Testing Logger: {}", "Variadic Message");
			Log::PrintMessage(std::source_location::current(), spdlog::level::level_enum::warn, "Testing Logger: {}", "Variadic Message");
			Log::PrintMessage(std::source_location::current(), spdlog::level::level_enum::err, "Testing Logger: {}", "Variadic Message");
			Log::PrintMessage(std::source_location::current(), spdlog::level::level_enum::critical, "Testing Logger: {}", "Variadic Message");
		}

		if (ImGui::BeginChild("Log Messages")) {
			for (const ImGuiLog& log : s_DisplayLogs) {
				ImGui::PushStyleColor(ImGuiCol_Text, log.Color);
				ImGui::TextWrapped(log.Message.c_str());
				ImGui::PopStyleColor();
			}
			ImGui::EndChild();
		}

		ImGui::PopStyleColor();
		ImGui::End();
	}

	void ImGuiLogs::AddLog(spdlog::level::level_enum level, std::string_view message)
	{
		std::tm tm = {};
		std::stringstream ss(message.data());
		std::string time;
		std::string milli;
		std::getline(ss, time, '[');
		std::getline(ss, time, '.');
		std::getline(ss, milli, ']');
		ss = std::stringstream(time);
		ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");

		int milliseconds = std::stoi(milli);
		std::chrono::time_point<std::chrono::system_clock> tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));
		tp += std::chrono::milliseconds(milliseconds);

		ImGuiLog log{ message.data(), GetColorByLevel(level), level, tp};
		std::vector<ImGuiLog>& logs = GetLogVector(level);

		if (s_DisplayLogs.size() >= 1000) {
			if (level >= s_Filter) {
				s_DisplayLogs.erase(s_DisplayLogs.begin());
			}
			logs.erase(logs.begin());
		}

		if (level >= s_Filter) {
			s_DisplayLogs.emplace_back(log);
		}
		logs.emplace_back(log);
	}

	void ImGuiLogs::FilterLogs()
	{
		s_DisplayLogs.clear();

		for (int filter = s_Filter; filter <= FilterLevel::Error; filter++) {
			switch (filter) {
			case FilterLevel::Debug:
				s_DisplayLogs.insert(s_DisplayLogs.end(), s_TraceLogs.begin(), s_TraceLogs.end());
				s_DisplayLogs.insert(s_DisplayLogs.end(), s_DebugLogs.begin(), s_DebugLogs.end());
				break;
			case FilterLevel::Info:
				s_DisplayLogs.insert(s_DisplayLogs.end(), s_InfoLogs.begin(), s_InfoLogs.end());
				break;
			case FilterLevel::Warning:
				s_DisplayLogs.insert(s_DisplayLogs.end(), s_WarnLogs.begin(), s_WarnLogs.end());
				break;
			case FilterLevel::Error:
				s_DisplayLogs.insert(s_DisplayLogs.end(), s_ErrorLogs.begin(), s_ErrorLogs.end());
				s_DisplayLogs.insert(s_DisplayLogs.end(), s_CriticalLogs.begin(), s_CriticalLogs.end());
				break;
			default:
				break;
			}
		}

		std::sort(s_DisplayLogs.begin(), s_DisplayLogs.end());
	}

	FilterLevel ImGuiLogs::GetFilterByName(std::string_view name)
	{
		if (name == "Debug") {
			return FilterLevel::Debug;
		}
		if (name == "Info") {
			return FilterLevel::Info;
		}
		if (name == "Warning") {
			return FilterLevel::Warning;
		}
		if (name == "Error") {
			return FilterLevel::Error;
		}
		return FilterLevel::Info;
	}

	std::vector<ImGuiLog>& ImGuiLogs::GetLogVector(spdlog::level::level_enum level)
	{
		switch (level) {
		case spdlog::level::level_enum::trace:
			return s_TraceLogs;
		case spdlog::level::level_enum::debug:
			return s_DebugLogs;
		case spdlog::level::level_enum::info:
			return s_InfoLogs;
			break;
		case spdlog::level::level_enum::warn:
			return s_WarnLogs;
			break;
		case spdlog::level::level_enum::err:
			return s_ErrorLogs;
			break;
		case spdlog::level::level_enum::critical:
			return s_CriticalLogs;
			break;
		default:
			return s_InfoLogs;
		}
	}
}

ImU32 GetColorByLevel(spdlog::level::level_enum level)
{
	switch (level) {
	case spdlog::level::level_enum::trace:
		return COLOR_TRACE;
	case spdlog::level::level_enum::debug:
		return COLOR_DEBUG;
	case spdlog::level::level_enum::info:
		return COLOR_INFO;
	case spdlog::level::level_enum::warn:
		return COLOR_WARN;
	case spdlog::level::level_enum::err:
		return COLOR_ERR;
	case spdlog::level::level_enum::critical:
		return COLOR_CRITICAL;
	default:
		return COLOR_INFO;
	}
}