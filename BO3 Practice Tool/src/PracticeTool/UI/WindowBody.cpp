#include "WindowBody.h"

#include <unordered_map>
#include <functional>
#include <iostream>
#include <stdexcept>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "Walnut/UI/UI.h"
#include "Walnut/ApplicationGUI.h"
#include "Embed/IcoMoon.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "misc/cpp/imgui_stdlib.h"

#include "PracticeTool/StructDefs/BGBData.h"
#include "PracticeTool/StructDefs/WeaponData.h"

namespace BO3PracticeTool
{
    struct TabBody {
        std::function<void()> BodyRender;
    };
    // Frontend Data
    void BGBPresetsBody();
    void WeaponPresetsBody();
    void AutosplitPresetsBody();
    void PracticePatchesBody();
    // In Game Options
    void PlayerOptionsBody();
    void ZombieOptionsBody();
    void RoundOptionsBody();
    void PowerupOptionsBody();
    void EggStepOptionsBody();
    void CraftableOptionsBody();
    void BlockerOptionsBody();
    void MapOptionsBody();
    // Resources
    void GumTrackerBody();
    void ZombieCalculatorBody();
    void CodeGuidesBody();
    void GKValveSolverBody();

    bool gumSelection = false;
    std::vector<std::string> bgbFilters = { "Alphabetical", "Rarity", "Color" };
    int bgbFilterIndex = 0;

    const std::unordered_map<int, std::vector<TabBody>> tabBodies = {
        {
            0, { { BGBPresetsBody }, { WeaponPresetsBody }, { AutosplitPresetsBody }, { PracticePatchesBody } },
        },
        {
            1, { { PlayerOptionsBody }, { ZombieOptionsBody }, { RoundOptionsBody }, { PowerupOptionsBody }, { EggStepOptionsBody }, { CraftableOptionsBody }, { BlockerOptionsBody }, { MapOptionsBody } },
        },
        {
            2, { { GumTrackerBody }, { ZombieCalculatorBody }, { CodeGuidesBody }, { GKValveSolverBody } },
        }
    };

    // TODO: Add logging on fail
    void RenderWindowBody(int TabGroup, int TabItem)
    {
        auto iter = tabBodies.find(TabGroup);
        if (iter != tabBodies.end()) {
            const std::vector<TabBody>& items = iter->second;
            if (TabItem <= items.size()) {
                items[TabItem].BodyRender();
            }
        }
    }
    // Frontend Data
    void BGBPresetsBody()
    {
        float width = ImGui::GetContentRegionAvail().x;
        static float editorWidth = width * 0.2f;
        static float bodyWidth = width * 0.8f;
        ImVec2 editorSize(editorWidth, -1);
        ImVec2 itemSpacing = ImGui::GetStyle().ItemSpacing;
        ImGui::Splitter("##Files Split", true, 8.0f, &editorWidth, &bodyWidth, 10.0f, 10.0f);

        ImGui::BeginChild("BGBPresets", editorSize);
        ImGui::HelpMarker("Right click to popup a menu. This section is essentially a file explorer.");
        bgbFiles.Render();
        ImGui::EndChild();

        if (!bgbPresets.size()) {
            return;
        }

        ImGui::SameLine();
        Walnut::UI::ShiftCursorX(10.0f);
        ImGui::BeginGroup();

        float heightAvail = ImGui::GetContentRegionAvail().y - itemSpacing.y * 4;
        ImVec2 imageSize(heightAvail / 5.0f, heightAvail / 5.0f);
        if (!gumSelection) {
            for (size_t i = 0; i < 5; i++) {
                const BGB* bgb = bgbPresets[bgbPresetsIndex].GetBGB(i);
                const ImTextureID& image = bgbCombinedImages[bgbNameToIndex[bgb->Name]]->Image->GetDescriptorSet();
                if (bgbMainGums.Render(i, image, imageSize).Hovered) {
                    bgbContextGum = bgbNameToIndex[bgb->Name];
                }
            }
        }

        ImGui::EndGroup();

        ImGui::SameLine();
        Walnut::UI::ShiftCursor(30.0f, -40.0f);
        ImGui::BeginGroup();
        
        float widthAvail = ImGui::GetContentRegionAvail().x - itemSpacing.x * 6 - ImGui::GetStyle().ScrollbarSize;
        if (ImGui::GetScrollMaxY() > 0) {
            widthAvail += ImGui::GetStyle().ScrollbarSize;
        }
        imageSize = imageSize * 0.8f;
        int numItems = static_cast<int>(ImGui::GetContentRegionAvail().x / (imageSize.x + itemSpacing.x));

        ImGui::PushFont(Walnut::Application::GetFont("Bold"));
        ImGui::Text("Currently Editing: %s", bgbPresets[bgbPresetsIndex].GetPresetName().c_str());
        ImGui::PopFont();
        ImGui::SameLine();
        ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
        ImGui::SameLine();

        static std::string searchText;
        ImGui::Text("Gum Search");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(widthAvail * 0.4f);
        if (ImGui::InputText("##Gum Search", &searchText)) {
            bgbClassicSelection ? CopyToDisplayImages(bgbClassicImages) : CopyToDisplayImages(bgbMegaImages);
            if (!searchText.empty()) {
                bgbDisplayImages = BGBSearch(bgbDisplayImages, searchText);
            }
        }
        ImGui::SameLine();
        ImGui::Text("Filter");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(200.0f);
        if (ImGui::BeginCombo("##Gum Filters", bgbFilters[bgbFilterIndex].c_str())) {
            if (ImGui::Selection::Render(bgbFilters, bgbFilterIndex)) {
                bgbSelectionFilter = static_cast<BGBFilter>(bgbFilterIndex);
                std::sort(bgbDisplayImages.begin(), bgbDisplayImages.end(), CompareImagePointer);
            }
            ImGui::EndCombo();
        }

        if (ImGui::BeginTabBar("Gum Selection Menus")) {
            if (ImGui::TabItemButton("Classics")) {
                bgbClassicSelection = true;
                bgbDisplayImages = BGBSearch(bgbDisplayImages, searchText);
            }
            if (ImGui::TabItemButton("Megas")) {
                bgbClassicSelection = false;
                bgbDisplayImages = BGBSearch(bgbDisplayImages, searchText);
            }
            ImGui::EndTabBar();
        }

        float height = ImGui::GetContentRegionAvail().y;
        float minSelectionHeight = std::max(height * 0.6f, height - 192);
        static float previousHeight = ImGui::GetContentRegionAvail().y;
        static float selectionHeight = height * 0.8f;
        static float contextHeight = height * 0.2f;
        if (height != previousHeight) {
            selectionHeight = std::max(minSelectionHeight, selectionHeight * (height / previousHeight));
            contextHeight = height - selectionHeight;
            previousHeight = height;
        }
        ImVec2 selectionSize(-1, selectionHeight);
        ImGui::Splitter("##Selection Split", false, 8.0f, &selectionHeight, &contextHeight, minSelectionHeight, 50.0f);

        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetStyle().Colors[ImGuiCol_WindowBg]);
        ImGui::BeginChild("Scrollable Gum Selection", ImVec2(-1, selectionHeight));
        for (size_t i = 0; i < bgbDisplayImages.size(); i++) {
            const BGBImage& image = *bgbDisplayImages[i];
            size_t addIndex = !bgbClassicSelection ? bgbClassicImages.size() : 0;
            if (ImGui::ImageButton(image.Image->GetDescriptorSet(), imageSize, ImVec2(0, 0), ImVec2(1, 1), 0)) {
                const size_t selected = bgbMainGums.GetSelected();
                BGB*& bgb = bgbPresets[bgbPresetsIndex].GetBGB(selected);
                bgb = &bgbs[i + addIndex];
                bgbPresets[bgbPresetsIndex].SavePreset();
            }
            if (bgbContextGum != i + addIndex && ImGui::IsItemHovered()) {
                bgbContextGum = i + addIndex;
            }
            if ((i + 1) % numItems || !i) {
                ImGui::SameLine();
            }
        }
        ImGui::EndChild();

        ImGui::GetCursorPos();
        float contextSize = ImGui::GetContentRegionAvail().y;
        ImGui::Image(bgbCombinedImages[bgbContextGum]->Image->GetDescriptorSet(), ImVec2(contextSize, contextSize));
        ImGui::SameLine();
        ImGui::BeginGroup();
        Walnut::UI::ShiftCursorY(contextSize * 0.2f);
        ImGui::PushFont(Walnut::Application::GetFont("Bold"));
        ImGui::Text(bgbs[bgbContextGum].Name.c_str());
        ImGui::PopFont();
        ImGui::TextWrapped(bgbs[bgbContextGum].Description.c_str());
        ImGui::EndGroup();

        ImGui::PopStyleColor();
        ImGui::EndGroup();
    }

    void WeaponPresetsBody()
    {
        float width = ImGui::GetContentRegionAvail().x;
        static float editorWidth = width * 0.2f;
        static float bodyWidth = width * 0.8f;
        ImVec2 editorSize(editorWidth, -1);
        ImVec2 itemSpacing = ImGui::GetStyle().ItemSpacing;
        ImGui::Splitter("##Files Split", true, 8.0f, &editorWidth, &bodyWidth, 10.0f, 10.0f);

        ImGui::BeginChild("WeaponPresets", editorSize);
        ImGui::HelpMarker("Right click to popup a menu. This section is essentially a file explorer.");
        weaponFiles.Render();
        ImGui::EndChild();

        if (!weaponPresets.size()) {
            return;
        }

        ImGui::SameLine();
        Walnut::UI::ShiftCursor(30.0f, -40.0f);

        ImGui::BeginGroup();

        ImGui::PushFont(Walnut::Application::GetFont("Bold"));
        ImGui::Text("Currently Editing: %s", weaponPresets[weaponPresetsIndex].GetPresetName().c_str());
        ImGui::PopFont();

        ImGui::BeginGroup();

        if (ImGui::BeginTabBar("Weapon Type Menus")) {
            for (size_t i = 0; i < weaponList.Types.size(); i++) {
                const std::string& type = weaponList.Types[i];
                if (ImGui::TabItemButton(type.c_str())) {
                    weaponPresetsMenu = i;
                    weaponBeingEdited = 0;
                    weaponMainWeapons.SetSelected(0);
                }
            }
            ImGui::EndTabBar();
        }

        const std::string& type = weaponList.Types[weaponPresetsMenu];
        const WeaponItem& item = weaponPresets[weaponPresetsIndex].GetWeaponItem(type, weaponBeingEdited);
        const Weapon& weapon = weaponList.Weapons[weaponPresetsMenu][weaponBeingEdited];
        const std::string& name = weapon.Name;
        const size_t& numAttachments = weapon.NumAttachments;
        const std::vector<std::string>& optics = weapon.Optics;
        const std::vector<std::string>& attachments = weapon.Attachments;

        float widthAvail = ImGui::GetContentRegionAvail().x;
        float imageWidth = std::min(widthAvail / 7 - itemSpacing.x, 192.0f);
        ImVec2 imageSize(imageWidth, imageWidth);
        for (size_t i = 0; i < optics.size(); i++) {
            const ImTextureID& image = opticsImgList[optics[i]]->GetDescriptorSet();
            const std::string opticName = optics[i];
            weaponOpticSelection.Render(i, image, imageSize, true);
            ImVec2 last(ImGui::GetItemRectMin().x + 5, ImGui::GetItemRectMax().y - 10);
            last.y -= ImGui::CalcTextSize(opticName.c_str()).y;
            ImGui::TextBackground(last, opticName);
            if (i < optics.size() - 1) {
                ImGui::SameLine();
            }
        }
        for (size_t i = 0; i < attachments.size(); i++) {
            const ImTextureID& attachment = BO3PracticeTool::attachmentsImgList[std::format("{}_{}", name, attachments[i])]->GetDescriptorSet();
            const std::string attachmentName = attachments[i];
            weaponAttachmentsSelection.Render(i, attachment, imageSize);
            ImVec2 last(ImGui::GetItemRectMin().x + 5, ImGui::GetItemRectMax().y - 10);
            last.y -= ImGui::CalcTextSize(attachmentName.c_str()).y;
            ImGui::TextBackground(last, attachmentName);
            if (i < attachments.size() - 1) {
                ImGui::SameLine();
            }
        }

        if (item.Camo >= 0) {
            if (ImGui::ImageButton(camosImgList[item.Camo].Image->GetDescriptorSet(), imageSize, ImVec2(0, 0), ImVec2(1, 1), 0)) {

            }
            ImVec2 last(ImGui::GetItemRectMin().x + 5, ImGui::GetItemRectMax().y - 10);
            last.y -= ImGui::CalcTextSize(camosImgList[item.Camo].Name.c_str()).y;
            ImGui::TextBackground(last, camosImgList[item.Camo].Name.c_str());
        }
        else {
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
            if (ImGui::Button("##CamoButton", imageSize)) {

            }
            ImVec2 last(ImGui::GetItemRectMin().x + 5, ImGui::GetItemRectMax().y - 10);
            last.y -= ImGui::CalcTextSize("Choose Camo").y;
            ImGui::TextBackground(last, "Choose Camo");
            ImGui::PopStyleVar();
        }

        Walnut::UI::ShiftCursorY(ImGui::GetContentRegionAvail().y - imageSize.y);

        const std::vector<Weapon>& weapons = weaponList.Weapons[weaponPresetsMenu];

        for (size_t i = 0; i < weapons.size(); i++) {
            const ImTextureID& weapon = weaponIconsImgList[weapons[i].Name]->GetDescriptorSet();
            const std::string weapontName = weapons[i].Name;
            if (weaponMainWeapons.Render(i, weapon, imageSize).Pressed) {
                weaponBeingEdited = i;
            }
            ImVec2 last(ImGui::GetItemRectMin().x + 5, ImGui::GetItemRectMax().y - 10);
            last.y -= ImGui::CalcTextSize(weapontName.c_str()).y;
            ImGui::TextBackground(last, weapontName);
            if (i < weapons.size() - 1) {
                ImGui::SameLine();
            }
        }

        ImGui::EndGroup();
        ImGui::EndGroup();
    }

    void AutosplitPresetsBody()
    {

    }

    void PracticePatchesBody()
    {

    }

    // In Game Options
    void PlayerOptionsBody()
    {

    }

    void ZombieOptionsBody()
    {

    }

    void RoundOptionsBody()
    {

    }

    void PowerupOptionsBody()
    {

    }

    void EggStepOptionsBody()
    {

    }

    void CraftableOptionsBody()
    {

    }

    void BlockerOptionsBody()
    {

    }

    void MapOptionsBody()
    {

    }

    // Resources
    void GumTrackerBody()
    {

    }

    void ZombieCalculatorBody()
    {

    }

    void CodeGuidesBody()
    {

    }

    void GKValveSolverBody()
    {

    }
}