#include "ImGuiHelper.h"

#define ICON_FA_ARROW_LEFT "\xef\x81\xa0"

namespace ImGuiHelper {

	PopupStates PopupWrapper::s_State;

	void MultiSpanSelection::Render(int selectionIndex)
	{
		int prevItem = m_Index;

		int index = 0;
		for (int i = 0; i < selectionIndex; i++) {
			index += m_Selections[i].m_Items.size();
		}

		for (int i = index; i < index + m_Selections[selectionIndex].m_Items.size(); i++) {
			const bool is_selected = m_Index == i;
			if (ImGui::Selectable(m_Selections[selectionIndex].m_Items[i - index].c_str(), is_selected)) {
				m_Index = i;
			}
			if (is_selected) {
				ImGui::SetItemDefaultFocus();
			}
		}

		if (prevItem != m_Index) {
			m_Changed = true;
		}
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

}