#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <iomanip>
#include "Walnut/Image.h"

namespace BO3PT
{
	inline bool done = false;
	inline bool enabled = false;
	inline std::string appStatus = "Status: Inactive";
	inline std::string currentMap = "zm_castle";
	inline std::string bo3Directory;
	inline std::string selfDirectory;
	inline std::string downloadURL;
	inline std::vector<Walnut::Image*> bgbImgList;
	inline std::vector<Walnut::Image*> codeImgList;
	inline std::vector<Walnut::Image*> valveSolverImgList;
	inline std::unordered_map<std::string, Walnut::Image*> iceCodeImgList;

	constexpr unsigned long hashstr(const char* str, int h = 0) { return !str[h] ? 55 : (hashstr(str, h + 1) * 33) + (unsigned char)(str[h]); }
	std::string ParseTimeFromMilli(int milliseconds);
	template <typename T>
	std::string CommifyNumString(T value)
	{
		std::stringstream ss;
		ss << std::fixed << std::setprecision(3) << value;

		std::string str = ss.str();
		std::locale locale;
		std::stringstream result;

		bool isNegative = false;
		if (str[0] == '-')
		{
			isNegative = true;
			result << '-';
			str.erase(0, 1);
		}

		size_t decimalPos = str.find('.');
		if (decimalPos == std::string::npos)
			decimalPos = str.length();

		for (size_t i = 0; i < str.length(); ++i)
		{
			if (i > 0 && i < decimalPos && (decimalPos - i) % 3 == 0)
				result << std::use_facet<std::numpunct<char>>(locale).thousands_sep();
			result << str[i];
		}

		return result.str();
	}
}