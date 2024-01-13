#pragma once

#include <Windows.h>
#include <string>

namespace Registry
{
	LONG GetDWORDRegKey(HKEY hKey, std::string_view strSubKey, std::string_view strValue, DWORD& nValue);
	LONG GetStringRegKey(HKEY hKey, std::string_view strSubKey, std::string_view strValue, std::string& strData);
	LONG CreateDWORDRegKey(HKEY hKey, std::string_view strSubKey, std::string_view strValue, DWORD dwData);
}