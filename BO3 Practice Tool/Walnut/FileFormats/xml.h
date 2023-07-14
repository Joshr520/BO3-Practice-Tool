#pragma once

#include <rapidxml/rapidxml.hpp>
#include <string>

namespace Walnut
{
    class XMLBuilder
    {
    public:
        XMLBuilder(std::string_view version, std::string_view type);
        ~XMLBuilder();

        rapidxml::xml_node<>* AddNode(rapidxml::xml_node<>* parent, const std::string& name, const std::string& value = "");
        void AddAttribute(rapidxml::xml_node<>* node, const std::string& name, const std::string& value);

        rapidxml::xml_node<>* GetRootNode();
        rapidxml::xml_document<>* GetDocument();

        void SaveToFile(const std::string& filename);
    private:
        rapidxml::xml_document<>* m_Doc;
        rapidxml::xml_node<>* m_Root;
    };
}