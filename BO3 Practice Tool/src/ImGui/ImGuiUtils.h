#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_internal.h"
#include "strnatcmp.h"

#include <string>
#include <format>
#include <queue>
#include <functional>

namespace ImGui
{
	namespace Colors
	{
		constexpr auto link = IM_COL32(5, 128, 255, 255);
	}

	// Popups
	void OpenOverridePopup(std::string_view name);
	bool BeginOverridePopup(std::string_view name, ImGuiWindowFlags flags = 0);
	void EndOverridePopup();
	bool BeginCenteredOverridePopupModal(std::string_view name, const ImVec2& sizeArg = ImVec2(0, 0), bool* open = NULL, ImGuiWindowFlags flags = 0);
	// Text
	void TextURL(std::string_view text, std::string_view url, bool continueBefore = 1, bool continueAfter = 1);
	void TextBold(const char* fmt, ...);
	void TextBackground(const ImVec2& start, std::string_view text);
	void TextClipped(std::string_view text, const ImRect& bb);
	void CenterText(std::string_view text);
	void CenterTextHorizontalToWindow(std::string_view text);
	// Selections
	bool Selection(const std::vector<std::string>& items, size_t& index);
	// Buttons
	void BeginRedButtons();
	void EndRedButtons();
}