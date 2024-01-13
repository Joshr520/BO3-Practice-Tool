#include "Hash.h"

#include <fstream>
#include <openssl/md5.h>
#include <sstream>

namespace Hash
{
	std::string GetFileHash(const std::filesystem::path& filePath)
	{
		std::ifstream file(filePath, std::ios::binary);

		if (!file.is_open()) {
			return "";
		}

		MD5_CTX md5Context;
		MD5_Init(&md5Context);

		const size_t bufferSize = 8192;
		char buffer[bufferSize];

		while (file.read(buffer, bufferSize)) {
			MD5_Update(&md5Context, buffer, file.gcount());
		}

		unsigned char hash[MD5_DIGEST_LENGTH];
		MD5_Final(hash, &md5Context);

		std::stringstream ss;
		for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
			ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
		}

		return ss.str();
	}
}