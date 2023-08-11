#include "json.h"

#include <rapidjson/error/en.h>
#include <rapidjson/prettywriter.h>
#include <fstream>
#include <sstream>

namespace BO3PracticeTool
{
    JSON JSON::FromFile(std::string_view filename)
    {
        std::ifstream inFile(filename.data());
        std::stringstream buffer;
        buffer << inFile.rdbuf();
        inFile.close();
        JSON json;
        json.GetDocument().Parse(buffer.str().c_str());
        if (json.GetDocument().HasParseError()) {
            json.GetDocument().SetObject();
            json.WriteEmpty(filename);
        }
        return json;
    }

    JSON JSON::FromString(const std::string& buffer)
    {
        JSON json;
        json.GetDocument().Parse(buffer.c_str());
        if (json.GetDocument().HasParseError()) {
            json.GetDocument().SetObject();
        }
        return json;
    }

    rapidjson::Value& JSON::AddObject(rapidjson::Value& parent, std::string_view keyName)
    {
        rapidjson::Value key(keyName.data(), m_Document.GetAllocator());
        rapidjson::Value value(rapidjson::kObjectType);
        parent.AddMember(key, value, m_Document.GetAllocator());
        return parent[keyName.data()];
    }

    rapidjson::Value& JSON::AddArray(rapidjson::Value& parent, std::string_view keyName)
    {
        rapidjson::Value key(keyName.data(), m_Document.GetAllocator());
        rapidjson::Value value(rapidjson::kArrayType);
        parent.AddMember(key, value, m_Document.GetAllocator());
        return parent[keyName.data()];
    }

    bool JSON::ModifyString(rapidjson::Value& parent, std::string_view keyName, std::string_view newValue)
    {
        if (!IsString(parent, keyName)) {
            return false;
        }
        parent[keyName.data()].SetString(rapidjson::StringRef(newValue.data()));
        return true;
    }

    bool JSON::ModifyInt(rapidjson::Value& parent, std::string_view keyName, int newValue)
    {
        if (!IsInt(parent, keyName)) {
            return false;
        }
        parent[keyName.data()].SetInt(newValue);
        return true;
    }

    bool JSON::ModifyDouble(rapidjson::Value& parent, std::string_view keyName, double newValue)
    {
        if (!IsDouble(parent, keyName)) {
            return false;
        }
        parent[keyName.data()].SetDouble(newValue);
        return true;
    }

    bool JSON::ModifyBool(rapidjson::Value& parent, std::string_view keyName, bool newValue)
    {
        if (!IsBool(parent, keyName)) {
            return false;
        }
        parent[keyName.data()].SetBool(newValue);
        return true;
    }

    void JSON::SaveToFile(std::string_view filekeyName)
    {
        std::ofstream outFile(filekeyName.data());
        rapidjson::StringBuffer sb;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
        m_Document.Accept(writer);
        outFile << sb.GetString() << std::endl;
        outFile.close();
    }

    void JSON::WriteEmpty(std::string_view filekeyName)
    {
        rapidjson::Document doc;
        doc.SetObject();
        std::ofstream outFile(filekeyName.data());
        rapidjson::StringBuffer sb;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
        doc.Accept(writer);
        outFile << sb.GetString() << std::endl;
        outFile.close();
    }
}