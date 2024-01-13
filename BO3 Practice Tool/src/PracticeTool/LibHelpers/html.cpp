#include "html.h"

#include <filesystem>

#include "curl/curl.h"
#include "PracticeTool/Core/Log.h"

namespace BO3PracticeTool
{
	size_t WriteToFile(void* contents, size_t size, size_t nmemb, FILE* stream)
	{
		return fwrite(contents, size, nmemb, stream);
	}

	bool DownloadFromURL(std::string_view url, std::string_view location)
	{
		if (!std::filesystem::exists(location)) {
			std::filesystem::create_directories(location);
		}

		curl_global_init(CURL_GLOBAL_ALL);
		CURL* curl = curl_easy_init();
		CURLcode res;
		FILE* file;

		errno_t err = fopen_s(&file, location.data(), "wb");
		if (err != 0) {
			char msg[256];
			strerror_s(msg, sizeof(msg), err);
			LOG_ERROR("Opening file {} failed with error code: {}", location, msg);
			return false;
		}

		curl_easy_setopt(curl, CURLOPT_URL, url.data());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteToFile);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		curl_global_cleanup();
		if (res != CURLE_OK) {
			LOG_ERROR("curl download failed with error code: {}", res);
			return false;
		}
		return true;
	}
}