#pragma once

#include <rapidjson/document.h>
#include <string>

namespace Walnut
{
    class JSONBuilder
    {
    public:
        JSONBuilder() { m_Document.SetObject(); }
        static JSONBuilder FromFile(std::string_view filename);
        static JSONBuilder FromString(const std::string& buffer);

        rapidjson::Value& AddObject(rapidjson::Value& parent, std::string_view keyName);
        rapidjson::Value& AddArray(rapidjson::Value& parent, std::string_view keyName);

        rapidjson::MemoryPoolAllocator<>& GetAllocator() { return m_Document.GetAllocator(); };
        rapidjson::Document& GetDocument() { return m_Document; };

        void ModifyString(rapidjson::Value& parent, std::string_view keyName, std::string_view newValue);
        void ModifyInt(rapidjson::Value& parent, std::string_view keyName, int newValue);
        void ModifyDouble(rapidjson::Value& parent, std::string_view keyName, double newValue);
        void ModifyBool(rapidjson::Value& parent, std::string_view keyName, bool newValue);

        void ModifyStringArray(rapidjson::Value& parent, int index, std::string_view newValue);
        void ModifyIntArray(rapidjson::Value& parent, int index, int newValue);
        void ModifyDoubleArray(rapidjson::Value& parent, int index, double newValue);
        void ModifyBoolArray(rapidjson::Value& parent, int index, bool newValue);

        void SaveToFile(std::string_view filekeyName);
        static void WriteEmpty(std::string_view filekeyName);

    private:
        rapidjson::Document m_Document;
    };
}