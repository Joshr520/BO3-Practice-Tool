#include "ImGuiUtils.h"
#include "imgui_internal.h"
#include "misc/cpp/imgui_stdlib.h"

#include "Walnut/ImGui/ImGuiTheme.h"

#include <thread>
#include <Windows.h>

namespace ImGui
{
	PopupState PopupWrapper::s_PrepState;
	PopupState PopupWrapper::s_OpenState;
	std::queue<PopupState> PopupWrapper::s_StateQueue;

	void PopupWrapper::PrepPopup(PopupState state)
	{
		if (state == PopupState::None || state == PopupState::Open) {
			return;
		}
		if (s_PrepState == PopupState::None) {
			s_PrepState = state;
		}
		else {
			s_StateQueue.push(state);
		}
	}

	void PopupWrapper::OpenPopup(PopupState state)
	{
		if (state == PopupState::None || state == PopupState::Open) {
			return;
		}
		s_PrepState = PopupState::Open;
		s_OpenState = state;
	}

	void PopupWrapper::ClosePopup()
	{
		s_OpenState = PopupState::None;
		if (s_StateQueue.size()) {
			s_PrepState = s_StateQueue.front();
			s_StateQueue.pop();
		}
		else {
			s_PrepState = PopupState::None;
		}
	}

	void TextURL::RenderTextURL(std::string_view text, std::string_view url, bool continueBefore, bool continueAfter)
	{
		if (continueBefore) {
			SameLine();
		}

		PushStyleColor(ImGuiCol_Text, Colors::link);
		Text(text.data());
		PopStyleColor();
		if (IsItemHovered()) {
			SetMouseCursor(ImGuiMouseCursor_Hand);
			if (IsMouseReleased(ImGuiMouseButton_Left)) {
				std::thread([command = url]() { ShellExecuteA(NULL, "open", command.data(), NULL, NULL, SW_SHOWNORMAL); }).detach();
			}
			UnderlineText(Colors::link);
		}

		if (continueAfter) {
			SameLine();
		}
	}

	void TextURL::UnderlineText(ImColor color)
	{
		ImVec2 min = GetItemRectMin();
		ImVec2 max = GetItemRectMax();
		min.y = max.y;
		GetWindowDrawList()->AddLine(min, max, color, 1.0f);
	}

	void Text::CenterText(std::string_view text)
	{
		ImGuiStyle& style = GetStyle();

		float actualSize = CalcTextSize(text.data()).x + style.FramePadding.x * 2.0f;
		float avail = GetContentRegionAvail().x;

		float off = (avail - actualSize) * 0.5f;
		if (off > 0.0f) {
			SetCursorPosX(GetCursorPosX() + off);
		}
	}

	bool Selection::RenderSelection(const std::vector<std::string>& items, int& index)
	{
		int prevItem = index;
		for (size_t i = 0; i < items.size(); i++) {
			const bool selected = index == i;
			if (Selectable(items[i].c_str(), selected)) {
				index = static_cast<int>(i);
			}
			if (selected) {
				SetItemDefaultFocus();
			}
		}
		return prevItem != index;
	}

	TextEditSelectable::TextEditResponse TextEditSelectable::RenderTextEditSelectable()
	{
		const std::string oldText = m_Text;
		std::string newText = m_Text;
		bool textEdited = false;
		bool itemHovered = false;
		int pops = 0;
		if (!m_BeingEdited) {
			if (m_Selected) {
				PushStyleColor(ImGuiCol_HeaderActive, GetStyle().Colors[ImGuiCol_HeaderHovered]);
				PushStyleColor(ImGuiCol_Header, GetStyle().Colors[ImGuiCol_HeaderActive]);
			}
			else {
				PushStyleColor(ImGuiCol_HeaderActive, GetStyle().Colors[ImGuiCol_HeaderHovered]);
				PushStyleColor(ImGuiCol_HeaderHovered, GetStyle().Colors[ImGuiCol_Header]);
			}
			pops = 2;
		}
		if (m_BeingEdited) {
			const ImU32 bgCol = GetColorU32(ImGuiCol_HeaderHovered);
			const ImVec2 cursor = GetCursorScreenPos();
			ImRect frameBB = ImRect(cursor, ImVec2(GetContentRegionMaxAbs().x, cursor.y + GetFrameHeight()));
			frameBB.Min.x -= IM_FLOOR(GetCurrentWindow()->WindowPadding.x * 0.5f - 1.0f);
			frameBB.Max.x += IM_FLOOR(GetCurrentWindow()->WindowPadding.x * 0.5f);
			RenderFrame(frameBB.Min, frameBB.Max, bgCol, true, GetStyle().FrameRounding);
			RenderNavHighlight(frameBB, GetID(m_ID.c_str()), ImGuiNavHighlightFlags_TypeThin);
			SetNextItemWidth(min(CalcTextSize(m_EditingText.c_str()).x + 25.0f, GetContentRegionAvail().x));
			SetKeyboardFocusHere();
			InputText(m_ID.c_str(), &m_EditingText, ImGuiInputTextFlags_AutoSelectAll);

			if (IsKeyPressed(ImGuiKey_Enter, false)) {
				m_BeingEdited = false;
				if (m_EditingText != m_Text) {
					textEdited = true;
					newText = m_EditingText;
					m_Text = m_EditingText;
				}
			}
			else if (!IsItemHovered() && (IsMouseClicked(ImGuiMouseButton_Left) || IsMouseClicked(ImGuiMouseButton_Right))) {
				if (!frameBB.Contains(GetMousePos())) {
					m_Selected = false;
				}
				else {
					itemHovered = true;
				}
				m_BeingEdited = false;
				if (m_EditingText != m_Text) {
					textEdited = true;
					newText = m_EditingText;
					m_Text = m_EditingText;
				}
			}
		}
		else {
			// Hard coding indentation in text since not sure how to do it with imgui functions
			TreeNodeEx(std::format("  {}", m_Text).c_str(), ImGuiTreeNodeFlags_CollapsingHeader | ImGuiTreeNodeFlags_Leaf);
		}
		PopStyleColor(pops);

		return { textEdited, itemHovered ? itemHovered : IsItemHovered(), { oldText, newText } };
	}
}