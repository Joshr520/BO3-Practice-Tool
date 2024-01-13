#include "Registry.h"

namespace Registry
{
	LONG GetDWORDRegKey(HKEY hKey, std::string_view strSubKey, std::string_view strValue, DWORD& nValue)
	{
		DWORD dwBufferSize = sizeof(DWORD);
		DWORD nResult;
		LONG nError = RegGetValueA(hKey, strSubKey.data(), strValue.data(), RRF_RT_REG_DWORD, nullptr, &nResult, &dwBufferSize);
		if (ERROR_SUCCESS == nError) {
			nValue = nResult;
		}
		return nError;
	}

	LONG GetStringRegKey(HKEY hKey, std::string_view strSubKey, std::string_view strValue, std::string& strData)
	{
		CHAR szBuffer[512];
		DWORD dwBufferSize = sizeof(szBuffer);
		LONG nError = RegGetValueA(hKey, strSubKey.data(), strValue.data(), RRF_RT_REG_SZ, nullptr, szBuffer, &dwBufferSize);;
		if (ERROR_SUCCESS == nError) {
			strData = szBuffer;
		}
		return nError;
	}

	LONG CreateDWORDRegKey(HKEY hKey, std::string_view strSubKey, std::string_view strValue, DWORD dwData)
	{
		HKEY newKey;
		LONG nError = RegOpenKeyExA(hKey, strSubKey.data(), 0, KEY_SET_VALUE | KEY_CREATE_SUB_KEY, &newKey);
		if (nError != ERROR_SUCCESS) {
			nError = RegCreateKeyExA(hKey, strSubKey.data(), 0, nullptr, 0, KEY_SET_VALUE | KEY_CREATE_SUB_KEY, nullptr, &newKey, nullptr);
			if (nError != ERROR_SUCCESS) {
				return nError;
			}
		}
		nError = RegSetValueExA(newKey, strValue.data(), 0, REG_DWORD, reinterpret_cast<const BYTE*>(&dwData), sizeof(DWORD));
		RegCloseKey(newKey);
		return nError;
	}
}