#include "xml.h"
#include <rapidxml/rapidxml_utils.hpp>
#include <rapidxml/rapidxml_print.hpp>

#include <fstream>

namespace Walnut
{
    XMLBuilder::XMLBuilder(std::string_view version, std::string_view type)
        : m_Doc(new rapidxml::xml_document<>()), m_Root(nullptr)
    {
        rapidxml::xml_node<>* decl = m_Doc->allocate_node(rapidxml::node_declaration);
        decl->append_attribute(m_Doc->allocate_attribute("version", m_Doc->allocate_string(version.data())));
        decl->append_attribute(m_Doc->allocate_attribute("encoding", "UTF-8"));
        m_Doc->append_node(decl);

        m_Root = m_Doc->allocate_node(rapidxml::node_element, m_Doc->allocate_string(type.data()));
        m_Doc->append_node(m_Root);
    }

    XMLBuilder::~XMLBuilder()
    {
        m_Doc->clear();
        delete m_Doc;
    }

    rapidxml::xml_node<>* XMLBuilder::AddNode(rapidxml::xml_node<>* parent, const std::string& name, const std::string& value)
    {
        rapidxml::xml_node<>* node = m_Doc->allocate_node(rapidxml::node_element, m_Doc->allocate_string(name.data()), m_Doc->allocate_string(value.data()));
        parent->append_node(node);
        return node;
    }

    void XMLBuilder::AddAttribute(rapidxml::xml_node<>* node, const std::string& name, const std::string& value)
    {
        node->append_attribute(m_Doc->allocate_attribute(m_Doc->allocate_string(name.data()), m_Doc->allocate_string(value.data())));
    }

    rapidxml::xml_node<>* XMLBuilder::GetRootNode()
    {
        return m_Root;
    }

    rapidxml::xml_document<>* XMLBuilder::GetDocument()
    {
        return m_Doc;
    }

    void XMLBuilder::SaveToFile(const std::string& filename)
    {
        std::ofstream file(filename);
        std::string data;
        rapidxml::print(std::back_inserter(data), *m_Doc, 0);
        file << data;
    }
}