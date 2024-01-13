#pragma once

#include <string>
#include <filesystem>

namespace Hash
{
	std::string GetFileHash(const std::filesystem::path& filePath);
}