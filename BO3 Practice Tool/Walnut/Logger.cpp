#include "Logger.h"
#include "Fonts/Icons.h"
#include "Walnut/Application.h"

#include <sstream>
#include <mutex>
#include <chrono>

static bool scrollDown = false;
ImGuiWindowFlags logFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDocking;

namespace Walnut
{
	std::vector<Message> Logger::s_LogMessages;
	std::ofstream Logger::s_LogFile;
	std::string Logger::s_LogFilename;
	bool Logger::s_Collapsed;
	ImVec2 Logger::s_LogWindowSize;
	ImVec2 Logger::s_LogWindowFull;

	void Logger::InitLogger(std::string_view filename)
	{
		s_LogFilename = filename.data();
		s_LogFile.open(s_LogFilename);
		s_LogFile.close();
		s_LogWindowSize = LOG_WINDOW_COLLAPSED_SIZE;
		s_Collapsed = true;
		logFlags |= ImGuiWindowFlags_NoTitleBar;

		Log(MessageType::Success, "Logger Init Completed");
	}

	std::mutex logMutex;
	void Logger::Log(MessageType type, std::string_view message)
	{
		std::lock_guard<std::mutex> lock(logMutex);

		Message logEntry;
		std::stringstream logText;

		auto now = std::chrono::system_clock::now();
		auto nowTime = std::chrono::system_clock::to_time_t(now);
		std::tm nowLocalTime{};
		localtime_s(&nowLocalTime, &nowTime);
		auto current_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count() % 1000;

		logText << "|" << std::put_time(&nowLocalTime, "%Y-%m-%d %H:%M:%S") << "." << std::setw(3) << std::setfill('0') << current_milliseconds << "| ";

		switch (type) {
		case MessageType::Error:
			logText << "[Error] ";
			logEntry.m_Color = COLOR_ERROR;
			break;
		case MessageType::Warning:
			logText << "[Warning] ";
			logEntry.m_Color = COLOR_WARNING;
			break;
		case MessageType::Info:
			logText << "[Info] ";
			logEntry.m_Color = COLOR_INFO;
			break;
		case MessageType::Success:
			logText << "[Success] ";
			logEntry.m_Color = COLOR_SUCCESS;
			break;
		case MessageType::Debug:
			logText << "[Debug] ";
			logEntry.m_Color = COLOR_DEBUG;
			break;
		default:
			logText << "[Unknown] ";
			logEntry.m_Color = COLOR_UNKNOWN;
			break;
		}

		logText << message << "\n";

		logEntry.m_Message = logText.str();
		s_LogMessages.emplace_back(logEntry);

		s_LogFile.open(s_LogFilename, std::ios::app);
		s_LogFile.write(logEntry.m_Message.c_str(), logEntry.m_Message.size());
		s_LogFile.close();
	}

	void Logger::DrawLogWindow()
	{
		if (!s_Collapsed) {
			s_LogWindowSize = ImVec2(ImGui::GetMainViewport()->Size.x, ImGui::GetMainViewport()->Size.y / 2);
		}

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImVec2 pos = ImVec2(viewport->Pos.x + viewport->Size.x - s_LogWindowSize.x, viewport->Pos.y + viewport->Size.y - s_LogWindowSize.y);
		ImGui::SetNextWindowPos(pos);
		ImGui::SetNextWindowSize(s_LogWindowSize);
		ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.2f, 0.2f, 0.2f, 1.f));
		if (!s_Collapsed) {
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.05f, 0.05f, 0.05f, 0.9f));
		}
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Logs", NULL, logFlags);
		ImGui::PopStyleVar();
		ImGui::PopStyleColor();
		if (!s_Collapsed) {
			ImGui::PopStyleColor();
		}

		if (!s_Collapsed) {
			for (const Message& message : s_LogMessages) {
				ImGui::TextColored(message.m_Color, message.m_Message.c_str());
			}
		}

		ImGui::SetCursorPos(ImVec2(ImGui::GetWindowContentRegionMax().x - 25.0f, ImGui::GetWindowContentRegionMax().y + ImGui::GetScrollY() * 2 - 25.0f));
		if (ImGui::Button(ICON_FA_FILE, ImVec2(25.0f, 25.0f)) || ImGui::IsKeyPressed(ImGuiKey_GraveAccent, false)) {
			ToggleCollapsed();
		}

		ImGui::End();
	}

	void Logger::ToggleCollapsed()
	{
		s_Collapsed = !s_Collapsed;
		if (s_Collapsed) {
			s_LogWindowSize = LOG_WINDOW_COLLAPSED_SIZE;
			logFlags |= ImGuiWindowFlags_NoTitleBar;
		}
		else {
			logFlags &= ~ImGuiWindowFlags_NoTitleBar;
		}
	}
}