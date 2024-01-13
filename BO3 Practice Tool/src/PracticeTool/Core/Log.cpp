#include "Log.h"
#include "../UI/LogUI.h"

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

#include <source_location>
#include <filesystem>

#define WL_HAS_CONSOLE !WL_DIST

namespace BO3PracticeTool
{
	bool Log::s_LogInit = false;
	std::shared_ptr<spdlog::logger> Log::s_Logger;
	std::shared_ptr<spdlog::sinks::ringbuffer_sink_mt> Log::s_RingBuffer;
	std::function<void(spdlog::level::level_enum, std::string_view)> Log::s_Callback;

	void Log::Init()
	{
		s_RingBuffer = std::make_shared<spdlog::sinks::ringbuffer_sink_mt>(1);

		std::vector<spdlog::sink_ptr> sinks =
		{
			std::make_shared<spdlog::sinks::basic_file_sink_mt>("Logs/PracticeTool.log", true),
			s_RingBuffer,
#if WL_HAS_CONSOLE
			std::make_shared<spdlog::sinks::stdout_color_sink_mt>()
#endif
		};

		s_Logger = std::make_shared<spdlog::logger>("PracticeTool", sinks.begin(), sinks.end());

		s_Logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] [%s:%#] %v");
		s_Logger->set_level(spdlog::level::level_enum::trace);
		spdlog::register_logger(s_Logger);

		SetCallback(ImGuiLogs::AddLog);

		LOG_INFO("Logging initialized");
		s_LogInit = true;
	}

	void Log::Shutdown()
	{
		LOG_INFO("Logging shutdown");
		spdlog::drop_all();
	}
}