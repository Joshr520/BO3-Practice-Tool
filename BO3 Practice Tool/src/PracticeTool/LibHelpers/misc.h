#pragma once
#include "PracticeTool/Core/Log.h"
#include "PracticeTool/UI/Notifications.h"

#include <filesystem>

inline bool CheckDirectory(std::string_view directory)
{
	if (!std::filesystem::exists(directory)) {
		try {
			std::filesystem::create_directories(directory);
		}
		catch (const std::filesystem::filesystem_error& ex) {
			LOG_ERROR("Error creating directories: ", ex.what());
			BO3PracticeTool::InfoNotification::AddInfoNoti("Creating Directory Error", std::format("An error occurred while creating directory {}", directory));
			return false;
		}
	}
	return true;
}