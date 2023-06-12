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
	std::vector<Message> Logger::logMessages;
	std::ofstream Logger::logFile;
	std::string Logger::logFilename;
	bool Logger::collapsed;
	ImVec2 Logger::logWindowSize;
	ImVec2 Logger::logWindowFull;

	void Logger::InitLogger(std::string_view filename)
	{
		logFilename = filename.data();
		logFile.open(logFilename);
		logFile.close();
		logWindowSize = LOG_WINDOW_COLLAPSED_SIZE;
		collapsed = true;
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

		switch (type)
		{
		case MessageType::Error:
			logText << "[Error] ";
			logEntry.color = COLOR_ERROR;
			break;
		case MessageType::Warning:
			logText << "[Warning] ";
			logEntry.color = COLOR_WARNING;
			break;
		case MessageType::Info:
			logText << "[Info] ";
			logEntry.color = COLOR_INFO;
			break;
		case MessageType::Success:
			logText << "[Success] ";
			logEntry.color = COLOR_SUCCESS;
			break;
		case MessageType::Debug:
			logText << "[Debug] ";
			logEntry.color = COLOR_DEBUG;
			break;
		default:
			logText << "[Unknown] ";
			logEntry.color = COLOR_UNKNOWN;
			break;
		}

		logText << message << "\n";

		logEntry.message = logText.str();
		logMessages.emplace_back(logEntry);

		logFile.open(logFilename, std::ios::app);
		logFile.write(logEntry.message.c_str(), logEntry.message.size());
		logFile.close();
	}

	void Logger::DrawLogWindow()
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImVec2 pos = ImVec2(viewport->Pos.x + viewport->Size.x - logWindowSize.x, viewport->Pos.y + viewport->Size.y - logWindowSize.y);
		ImGui::SetNextWindowPos(pos);
		ImGui::SetNextWindowSize(logWindowSize);
		ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.2f, 0.2f, 0.2f, 1.f));
		if (!collapsed)
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.05f, 0.05f, 0.05f, 0.9f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Logs", NULL, logFlags);
		ImGui::PopStyleVar();
		ImGui::PopStyleColor();
		if (!collapsed)
			ImGui::PopStyleColor();

		if (!collapsed)
		{
			for (const Message& message : logMessages)
			{
				ImGui::TextColored(message.color, message.message.c_str());
			}
		}

		ImGui::SetCursorPos(ImVec2(ImGui::GetWindowContentRegionMax().x - 25.0f, ImGui::GetWindowContentRegionMax().y + ImGui::GetScrollY() * 2 - 25.0f));
		if (ImGui::Button(ICON_FA_FILE, ImVec2(25.0f, 25.0f)) || ImGui::IsKeyPressed(ImGuiKey_GraveAccent, false))
		{
			ToggleCollapsed();
		}

		ImGui::End();
	}

	void Logger::ToggleCollapsed()
	{
		collapsed = !collapsed;
		if (collapsed)
		{
			logWindowSize = LOG_WINDOW_COLLAPSED_SIZE;
			logFlags |= ImGuiWindowFlags_NoTitleBar;
		}
		else
		{
			logWindowSize = ImVec2(ImGui::GetMainViewport()->Size.x, ImGui::GetMainViewport()->Size.y / 2);
			logFlags &= ~ImGuiWindowFlags_NoTitleBar;
		}
	}
}