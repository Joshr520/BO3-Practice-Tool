#include "json.h"
#include "GlobalData.h"

using namespace BO3PT;

namespace JSON
{
    rapidjson::Document ReadJsonFromFile(const std::string& filename)
    {
        std::ifstream inFile(filename);
        std::stringstream buffer;
        buffer << inFile.rdbuf();
        inFile.close();
        Document doc;
        doc.Parse(buffer.str().c_str());
        if (doc.HasParseError())
        {
            doc.SetObject();

            Value steamPathValue(kStringType);
            steamPathValue.SetString("", doc.GetAllocator());
            doc.AddMember("Steam Path", steamPathValue, doc.GetAllocator());

            std::ofstream outFile(filename);
            StringBuffer sb;
            PrettyWriter<StringBuffer> writer(sb);
            doc.Accept(writer);
            outFile << sb.GetString() << std::endl;
            outFile.close();
        }

        return doc;
    }

    rapidjson::Document ReadJsonFromString(const std::string& buffer)
    {
        Document doc;
        doc.Parse(buffer.c_str());
        if (doc.HasParseError())
            doc.SetObject();
        return doc;
    }

    void WriteJson(const Document& doc, const std::string& filename)
    {
        std::ofstream outFile(filename);
        StringBuffer sb;
        PrettyWriter<StringBuffer> writer(sb);
        doc.Accept(writer);
        outFile << sb.GetString() << std::endl;
        outFile.close();
    }

    void WriteEmptyJson(const std::string& filename)
    {
        Document doc;
        StringBuffer sb;
        PrettyWriter<StringBuffer> writer(sb);
        doc.SetObject();
        doc.Accept(writer);
        std::ofstream outFile(filename);
        outFile << sb.GetString() << std::endl;
        outFile.close();
    }

    void ModifyJsonString(Document& doc, const std::string& key, const std::string& value)
    {
        if (!doc.HasMember(key.c_str()))
        {
            Value jsonValue;
            jsonValue.SetString(key.c_str(), key.size(), doc.GetAllocator());
            doc.AddMember(jsonValue, Value().SetString(StringRef(value.c_str(), value.size()), doc.GetAllocator()), doc.GetAllocator());
        }
        else
            doc[key.c_str()].SetString(StringRef(value.c_str()));
    }

    bool GetStringFromJsonArray(const std::string& array, const std::string& key, const Document& doc, std::string& buffer)
    {
        const Value& values = doc[array.c_str()];
        if (values.IsArray() && !values.Empty())
        {
            const Value& value = values[0];
            if (value.HasMember(key.c_str()) && value[key.c_str()].IsString())
                buffer = value[key.c_str()].GetString();
            else
            {
                LogFile("Missing or invalid " + key + " field");
                return 0;
            }
        }
        else
        {
            LogFile(key + " not found or invalid");
            return 0;
        }

        return 1;
    }

    bool GetStringFromJson(const std::string& key, const Document& doc, std::string& buffer)
    {
        const Value& value = doc[key.c_str()];
        if (value.IsString())
            buffer = value.GetString();
        else
        {
            LogFile(key + " not found or empty");
            return 0;
        }

        return 1;
    }
}