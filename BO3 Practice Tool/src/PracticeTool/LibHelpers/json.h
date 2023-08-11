#pragma once

#include <rapidjson/document.h>
#include <string>

namespace BO3PracticeTool
{
    template <typename T>
    struct JSONGetResult {
        bool GetSuccess;
        T ReturnValue;
    };

    class JSON
    {
    public:
        JSON() { m_Document.SetObject(); }
        static JSON FromFile(std::string_view filename);
        static JSON FromString(const std::string& buffer);

        inline static bool HasMember(const rapidjson::Value& parent, std::string_view keyName) { return parent.HasMember(keyName.data()); }
        inline static bool IsObject(const rapidjson::Value& parent, std::string_view keyName) { return parent.HasMember(keyName.data()) && parent[keyName.data()].IsObject(); }
        inline static bool IsArray(const rapidjson::Value& parent, std::string_view keyName) { return parent.HasMember(keyName.data()) && parent[keyName.data()].IsArray(); }
        inline static bool IsString(const rapidjson::Value& parent, std::string_view keyName) { return parent.HasMember(keyName.data()) && parent[keyName.data()].IsString(); }
        inline static bool IsInt(const rapidjson::Value& parent, std::string_view keyName) { return parent.HasMember(keyName.data()) && parent[keyName.data()].IsInt(); }
        inline static bool IsDouble(const rapidjson::Value& parent, std::string_view keyName) { return parent.HasMember(keyName.data()) && parent[keyName.data()].IsDouble(); }
        inline static bool IsBool(const rapidjson::Value& parent, std::string_view keyName) { return parent.HasMember(keyName.data()) && parent[keyName.data()].IsBool(); }
        inline static JSONGetResult<std::string> GetString(const rapidjson::Value& parent, std::string_view keyName)
        {
            return IsString(parent, keyName) ? JSONGetResult<std::string>{ true, parent[keyName.data()].GetString() } : JSONGetResult<std::string>{ false, "" };
        }
        inline static JSONGetResult<int> GetInt(const rapidjson::Value& parent, std::string_view keyName)
        {
            return IsInt(parent, keyName) ? JSONGetResult<int>{ true, parent[keyName.data()].GetInt() } : JSONGetResult<int>{ false, -1 };
        }
        inline static JSONGetResult<double> GetDouble(const rapidjson::Value& parent, std::string_view keyName)
        {
            return IsInt(parent, keyName) ? JSONGetResult<double>{ true, parent[keyName.data()].GetDouble() } : JSONGetResult<double>{ false, -1.0 };
        }
        inline static JSONGetResult<bool> GetBool(const rapidjson::Value& parent, std::string_view keyName)
        {
            return IsBool(parent, keyName) ? JSONGetResult<bool>{ true, parent[keyName.data()].GetBool() } : JSONGetResult<bool>{ false, false };
        }

        rapidjson::Value& AddObject(rapidjson::Value& parent, std::string_view keyName);
        rapidjson::Value& AddArray(rapidjson::Value& parent, std::string_view keyName);

        rapidjson::MemoryPoolAllocator<>& GetAllocator() { return m_Document.GetAllocator(); };
        rapidjson::Document& GetDocument() { return m_Document; };

        bool ModifyString(rapidjson::Value& parent, std::string_view keyName, std::string_view newValue);
        bool ModifyInt(rapidjson::Value& parent, std::string_view keyName, int newValue);
        bool ModifyDouble(rapidjson::Value& parent, std::string_view keyName, double newValue);
        bool ModifyBool(rapidjson::Value& parent, std::string_view keyName, bool newValue);

        void SaveToFile(std::string_view filekeyName);
        static void WriteEmpty(std::string_view filekeyName);

    private:
        rapidjson::Document m_Document;
    };
}