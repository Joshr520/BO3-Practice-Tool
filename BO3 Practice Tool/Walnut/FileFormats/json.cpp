#include "json.h"
#include "Walnut/Logger.h"

#include <rapidjson/error/en.h>
#include <rapidjson/prettywriter.h>
#include <fstream>
#include <sstream>

namespace Walnut
{
    JSONBuilder::JSONBuilder(std::string_view filename)
    {
        std::ifstream inFile(filename.data());
        std::stringstream buffer;
        buffer << inFile.rdbuf();
        inFile.close();
        m_Document.Parse(buffer.str().c_str());
        if (m_Document.HasParseError()) {
            m_Document.SetObject();
            WriteEmpty(filename);
        }
    }

    JSONBuilder::JSONBuilder(const std::string& buffer)
    {
        m_Document.Parse(buffer.c_str());
        if (m_Document.HasParseError()) {
            m_Document.SetObject();
        }
    }

    rapidjson::Value& JSONBuilder::AddObject(rapidjson::Value& parent, std::string_view keyName)
    {
        rapidjson::Value key(keyName.data(), m_Document.GetAllocator());
        rapidjson::Value value(rapidjson::kObjectType);
        parent.AddMember(key, value, m_Document.GetAllocator());
        return parent[keyName.data()];
    }

    rapidjson::Value& JSONBuilder::AddArray(rapidjson::Value& parent, std::string_view keyName)
    {
        rapidjson::Value key(keyName.data(), m_Document.GetAllocator());
        rapidjson::Value value(rapidjson::kArrayType);
        parent.AddMember(key, value, m_Document.GetAllocator());
        return parent[keyName.data()];
    }

    void JSONBuilder::ModifyString(rapidjson::Value& parent, std::string_view keyName, std::string_view newValue)
    {
        if (!parent.HasMember(keyName.data())) {
            rapidjson::Value value;
            value.SetString(keyName.data(), (rapidjson::SizeType)keyName.size(), m_Document.GetAllocator());
            parent.AddMember(value, rapidjson::Value().SetString(rapidjson::StringRef(newValue.data(), newValue.size()), m_Document.GetAllocator()), m_Document.GetAllocator());
        }
        else {
            parent[keyName.data()].SetString(rapidjson::StringRef(newValue.data()));
        }
    }

    void JSONBuilder::ModifyInt(rapidjson::Value& parent, std::string_view keyName, int newValue)
    {
        if (!parent.HasMember(keyName.data())) {
            rapidjson::Value value;
            value.SetString(keyName.data(), (rapidjson::SizeType)keyName.size(), m_Document.GetAllocator());
            parent.AddMember(value, rapidjson::Value().SetInt(newValue), m_Document.GetAllocator());
        }
        else {
            parent[keyName.data()].SetInt(newValue);
        }
    }

    void JSONBuilder::ModifyDouble(rapidjson::Value& parent, std::string_view keyName, double newValue)
    {
        if (!parent.HasMember(keyName.data())) {
            rapidjson::Value value;
            value.SetString(keyName.data(), (rapidjson::SizeType)keyName.size(), m_Document.GetAllocator());
            parent.AddMember(value, rapidjson::Value().SetDouble(newValue), m_Document.GetAllocator());
        }
        else {
            parent[keyName.data()].SetDouble(newValue);
        }
    }

    void JSONBuilder::ModifyBool(rapidjson::Value& parent, std::string_view keyName, bool newValue)
    {
        if (!parent.HasMember(keyName.data())) {
            rapidjson::Value value;
            value.SetString(keyName.data(), (rapidjson::SizeType)keyName.size(), m_Document.GetAllocator());
            parent.AddMember(value, rapidjson::Value().SetBool(newValue), m_Document.GetAllocator());
        }
        else {
            parent[keyName.data()].SetBool(newValue);
        }
    }

    void JSONBuilder::ModifyStringArray(rapidjson::Value& parent, int index, std::string_view newValue)
    {
        if (parent.IsArray() && index < (int)parent.Size()) {
            parent[index].SetString(rapidjson::StringRef(newValue.data()));
        }
    }

    void JSONBuilder::ModifyIntArray(rapidjson::Value& parent, int index, int newValue)
    {
        if (parent.IsArray() && index < (int)parent.Size()) {
            parent[index].SetInt(newValue);
        }
    }

    void JSONBuilder::ModifyDoubleArray(rapidjson::Value& parent, int index, double newValue)
    {
        if (parent.IsArray() && index < (int)parent.Size()) {
            parent[index].SetDouble(newValue);
        }
    }

    void JSONBuilder::ModifyBoolArray(rapidjson::Value& parent, int index, bool newValue)
    {
        if (parent.IsArray() && index < (int)parent.Size()) {
            parent[index].SetBool(newValue);
        }
    }

    void JSONBuilder::SaveToFile(std::string_view filekeyName)
    {
        std::ofstream outFile(filekeyName.data());
        rapidjson::StringBuffer sb;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
        m_Document.Accept(writer);
        outFile << sb.GetString() << std::endl;
        outFile.close();
    }

    void JSONBuilder::WriteEmpty(std::string_view filekeyName)
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