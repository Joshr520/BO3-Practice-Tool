#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/sinks/ringbuffer_sink.h"
#include "spdlog/fmt/fmt.h"

#include <source_location>

namespace BO3PracticeTool
{
	class Log {
	public:
		static void Init();
		static void Shutdown();

		static bool LogReady() { return s_LogInit; }

		static void SetCallback(std::function<void(spdlog::level::level_enum, std::string_view)> callback) { s_Callback = callback; }

		template<typename... Args>
		static void PrintMessage(const std::source_location& location, spdlog::level::level_enum level, std::string_view fmt, Args&&... args)
		{
			std::string message = fmt::vformat(fmt, fmt::make_format_args(args...));
			s_Logger->log(spdlog::source_loc{ location.file_name(), static_cast<int>(location.line()), location.function_name() }, level, message);
			s_Logger->flush();
			if (s_Callback) {
				s_Callback(level, s_RingBuffer->last_formatted()[0]);
			}
		}
	private:
		static bool s_LogInit;

		static std::shared_ptr<spdlog::logger> s_Logger;
		static std::shared_ptr<spdlog::sinks::ringbuffer_sink_mt> s_RingBuffer;

		static std::function<void(spdlog::level::level_enum, std::string_view)> s_Callback;
	};
}

#define LOG_TRACE(...)		BO3PracticeTool::Log::PrintMessage(std::source_location::current(), spdlog::level::level_enum::trace, __VA_ARGS__)
#define LOG_DEBUG(...)		BO3PracticeTool::Log::PrintMessage(std::source_location::current(), spdlog::level::level_enum::debug, __VA_ARGS__)
#define LOG_INFO(...)		BO3PracticeTool::Log::PrintMessage(std::source_location::current(), spdlog::level::level_enum::info, __VA_ARGS__)
#define LOG_WARN(...)		BO3PracticeTool::Log::PrintMessage(std::source_location::current(), spdlog::level::level_enum::warn, __VA_ARGS__)
#define LOG_ERROR(...)		BO3PracticeTool::Log::PrintMessage(std::source_location::current(), spdlog::level::level_enum::err, __VA_ARGS__)
#define LOG_CRITICAL(...)	BO3PracticeTool::Log::PrintMessage(std::source_location::current(), spdlog::level::level_enum::critical, __VA_ARGS__)