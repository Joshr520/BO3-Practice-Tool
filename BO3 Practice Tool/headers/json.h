#pragma once
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <sstream>
#include <fstream>

using namespace rapidjson;

namespace JSON
{
	rapidjson::Document ReadJsonFromFile(const std::string& filename);
	rapidjson::Document ReadJsonFromString(const std::string& buffer);
	void WriteJson(const Document& doc, const std::string& filename);
	void WriteEmptyJson(const std::string& filename);
	void ModifyJsonString(Document& doc, const std::string& key, const std::string& value);
	bool GetStringFromJsonArray(const std::string& array, const std::string& key, const Document& doc, std::string& buffer);
	bool GetStringFromJson(const std::string& key, const Document& doc, std::string& buffer);
}