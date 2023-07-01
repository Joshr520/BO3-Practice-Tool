#include "ImGuiHelper.h"

#define ICON_FA_ARROW_LEFT "\xef\x81\xa0"
#define ICON_FA_CIRCLE_QUESTION "\xef\x81\x99"

namespace ImGuiHelper {

	PopupStates PopupWrapper::s_PrepState;
	PopupStates PopupWrapper::s_OpenState;
	std::queue<PopupStates> PopupWrapper::s_StateQueue;

	void MultiSpanSelection::Render(int selectionIndex)
	{
		if (!m_Selections.size()) {
			return;
		}

		int prevItem = m_Index;

		int index = 0;
		for (int i = 0; i < selectionIndex; i++) {
			index += static_cast<int>(m_Selections[i].m_Items.size());
		}

		for (int i = index; i < index + m_Selections[selectionIndex].m_Items.size(); i++) {
			const bool selected = m_Index == i;
			if (ImGui::Selectable(m_Selections[selectionIndex].m_Items[i - index].c_str(), selected)) {
				m_Index = i;
			}
			if (selected) {
				ImGui::SetItemDefaultFocus();
			}
		}

		if (prevItem != m_Index) {
			m_Changed = true;
		}
	}

	int MultiSpanSelection::GetArrIndex() const
	{
		int indexArr = 0;
		int indexItem = 0;
		for (const SelectionData& data : m_Selections) {
			if (indexItem + static_cast<int>(data.m_Items.size()) < m_Index + 1) {
				indexItem += static_cast<int>(data.m_Items.size());
				indexArr++;
			}
			else {
				return indexArr;
			}
		}

		return -1;
	}

	std::pair<int, int> MultiSpanSelection::GetMultiIndex() const
	{
		int indexArr = 0;
		int indexItem = 0;
		for (const SelectionData& data : m_Selections) {
			if (indexItem + static_cast<int>(data.m_Items.size()) < m_Index + 1) {
				indexItem += static_cast<int>(data.m_Items.size());
				indexArr++;
			}
			else {
				return { indexArr, m_Index - indexItem };
			}
		}

		return { -1, -1 };
	}

	std::string MultiSpanSelection::GetItemAtIndex() const
	{
		int index = 0;
		for (const SelectionData& data : m_Selections) {
			if (index + static_cast<int>(data.m_Items.size()) < m_Index + 1) {
				index += static_cast<int>(data.m_Items.size());
			}
			else {
				return data.m_Items[m_Index - index];
			}
		}
		
		return "Not Found";
	}

	bool Selection::Render(const std::vector<std::string>& items, int& index)
	{
		int prevItem = index;

		for (int i = 0; i < static_cast<int>(items.size()); i++) {
			const bool selected = index == i;
			if (ImGui::Selectable(items[i].c_str(), selected)) {
				index = i;
			}
			if (selected) {
				ImGui::SetItemDefaultFocus();
			}
		}

		return prevItem != index;
	}

	bool Selection::RenderBGB(const std::vector<BGB>& items, int& index)
	{
		int prevItem = index;

		for (int i = 0; i < static_cast<int>(items.size()); i++) {
			const bool selected = index == i;
			if (ImGui::Selectable(items[i].m_Name.c_str(), selected)) {
				index = i;
			}
			if (selected) {
				ImGui::SetItemDefaultFocus();
			}
		}

		return prevItem != index;
	}

	bool Selection::RenderBGBPreset(const std::vector<BGBPreset>& items, int& index)
	{
		int prevItem = index;

		for (int i = 0; i < static_cast<int>(items.size()); i++) {
			const bool selected = index == i;
			if (ImGui::Selectable(items[i].m_Name.c_str(), selected)) {
				index = i;
			}
			if (selected) {
				ImGui::SetItemDefaultFocus();
			}
		}

		return prevItem != index;
	}

	bool Selection::RenderWeaponLoadout(const std::vector<MenuWeaponPreset>& items, int& index)
	{
		int prevItem = index;

		for (int i = 0; i < static_cast<int>(items.size()); i++) {
			const bool selected = index == i;
			if (ImGui::Selectable(items[i].m_Name.c_str(), selected)) {
				index = i;
			}
			if (selected) {
				ImGui::SetItemDefaultFocus();
			}
		}

		return prevItem != index;
	}

	void TextFont::Render(std::string_view text, ImFont* font, bool push, bool pop)
	{
		if (push) {
			ImGui::PushFont(font);
		}
		ImGui::Text(text.data());
		if (pop) {
			ImGui::PopFont();
		}
	}

	void TextFont::RenderWrapped(std::string_view text, ImFont* font, bool push, bool pop)
	{
		if (push) {
			ImGui::PushFont(font);
		}
		ImGui::TextWrapped(text.data());
		if (pop) {
			ImGui::PopFont();
		}
	}

	void PopupWrapper::PrepPopup(PopupStates state)
	{
		if (state == None || state == Open) {
			return;
		}
		if (s_PrepState == None) {
			s_PrepState = state;
		}
		else {
			s_StateQueue.push(state);
		}
	}

	void PopupWrapper::OpenPopup(PopupStates state)
	{
		if (state == None || state == Open) {
			return;
		}
		s_PrepState = Open;
		s_OpenState = state;
	}

	void PopupWrapper::ClosePopup()
	{
		s_OpenState = None;
		if (s_StateQueue.size()) {
			s_PrepState = s_StateQueue.front();
			s_StateQueue.pop();
		}
		else {
			s_PrepState = None;
		}
	}

	bool ButtonWrapper::RenderToggle(std::string_view name, const ImVec2& size, bool active, bool displayOnly, const ImU32& colorOn, const ImU32& colorOff)
	{
		ImU32 color = active ? colorOn : colorOff;

		ImGui::PushStyleColor(ImGuiCol_Button, color);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color);

		if (ImGui::Button(name.data(), size) && !displayOnly) {
			ImGui::PopStyleColor(2);
			return 1;
		}

		ImGui::PopStyleColor(2);
		return 0;
	}

	bool ButtonWrapper::RenderToggleOut(std::string_view name, const ImVec2& size, bool* active, bool displayOnly, const ImU32& colorOn, const ImU32& colorOff)
	{
		ImU32 color = *active ? colorOn : colorOff;

		ImGui::PushStyleColor(ImGuiCol_Button, color);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color);

		if (ImGui::Button(name.data(), size) && !displayOnly) {
			*active = !*active;
			ImGui::PopStyleColor(2);
			return 1;
		}

		ImGui::PopStyleColor(2);
		return 0;
	}

	bool ButtonWrapper::RenderSingle(std::string_view name, const ImVec2& size, bool displayOnly, const ImU32& color)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, color);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color);

		if (ImGui::Button(name.data(), size) && !displayOnly) {
			ImGui::PopStyleColor(2);
			return 1;
		}

		if (name == ICON_FA_ARROW_LEFT && ImGui::IsKeyPressed(ImGuiKey_Escape, false)) {
			ImGui::PopStyleColor(2);
			return 1;
		}

		ImGui::PopStyleColor(2);
		return 0;
	}

	void ButtonWrapper::RenderFake(std::string_view name, const ImVec2& size, const ImU32& color)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, color);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);
		ImGui::Button(name.data(), size);
		ImGui::PopStyleColor(3);
	}

	bool ListBoxWrapper::Render(std::string_view name, const std::vector<std::string>& items, int& index, const ImVec2& size)
	{
		bool changed = false;
		if (ImGui::BeginListBox(name.data(), size)) {
			if (Selection::Render(items, index)) {
				changed = true;
			}
			ImGui::EndListBox();
		}
		return changed;
	}

	void HelpMarker::Render(std::string_view text)
	{
		ImGui::TextDisabled(ICON_FA_CIRCLE_QUESTION);
		if (ImGui::IsItemHovered()) {
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextWrapped(text.data());
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}

}