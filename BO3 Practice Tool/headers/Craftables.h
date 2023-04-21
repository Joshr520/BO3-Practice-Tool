#pragma once

#include <unordered_map>
#include <vector>
#include <sstream>

namespace Craftables
{
    inline int craftListIndex = 0;
    inline int craftStaff = 0;
    inline std::unordered_map<std::string, std::unordered_map<int, std::vector<std::string>>> craftList;
    inline std::unordered_map<std::string, std::unordered_map<int, std::string>> craftNames;
    inline std::unordered_map<std::string, std::unordered_map<int, int>> craftComboIndexes;

    void InitCraftablesList();
}