#pragma once

#include <string>

namespace BO3PracticeTool
{
	bool DownloadFromURL(std::string_view url, std::string_view location);
}