#include "ImGuiUtils.h"
#include "imgui_internal.h"

#include "Walnut/ImGui/ImGuiTheme.h"
#include "Walnut/Application.h"

#include <thread>
#include <Windows.h>

namespace ImGui
{
	void UnderlineText(ImColor color);

	void OpenOverridePopup(std::string_view name)
	{
		ImGuiID id = ImHashStr(name.data());
		PushOverrideID(id);
		OpenPopup(name.data());
		PopID();
	}

	bool BeginOverridePopup(std::string_view name, ImGuiWindowFlags flags)
	{
		ImGuiID id = ImHashStr(name.data());
		PushOverrideID(id);
		if (!BeginPopup(name.data(), flags)) {
			PopID();
			return false;
		}
		return true;
	}

	void EndOverridePopup()
	{
		EndPopup();
		PopID();
	}

	bool BeginCenteredOverridePopupModal(std::string_view name, const ImVec2& sizeArg, bool* open, ImGuiWindowFlags flags)
	{
		ImGuiContext& g = *GImGui;
		ImGuiViewport* viewport = g.CurrentViewport;

		ImVec2 size = ImFloor(sizeArg);
		if (size.x <= 0.0f) {
			size.x = viewport->WorkSize.x * 0.6f;
		}
		if (size.y <= 0.0f) {
			size.y = viewport->WorkSize.y * 0.6f;
		}
		ImVec2 windowPos = ImVec2(viewport->WorkPos.x + (viewport->WorkSize.x - size.x) * 0.5f, viewport->WorkPos.y + (viewport->WorkSize.y - size.y) * 0.5f);

		SetNextWindowSize(size);
		SetNextWindowPos(windowPos, ImGuiCond_Always);

		ImGuiID id = ImHashStr(name.data());
		PushOverrideID(id);

		if (!BeginPopupModal(name.data(), open, flags)) {
			PopID();
			return false;
		}
		return true;
	}

	void TextURL(std::string_view text, std::string_view url, bool continueBefore, bool continueAfter)
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

	void UnderlineText(ImColor color)
	{
		ImVec2 min = GetItemRectMin();
		ImVec2 max = GetItemRectMax();
		min.y = max.y;
		GetWindowDrawList()->AddLine(min, max, color, 1.0f);
	}

	void TextBold(const char* fmt, ...)
	{
		PushFont(Walnut::Application::GetFont("Bold"));
		va_list args;
		va_start(args, fmt);
		TextV(fmt, args);
		va_end(args);
		PopFont();
	}

	void TextBackground(const ImVec2& start, std::string_view text)
	{
		ImGuiContext& g = *GImGui;
		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems) {
			return;
		}

		ImVec2 textSize = CalcTextSize(text.data());
		ImVec2 end(start + textSize + ImVec2(5.0f, 5.0f));

		window->DrawList->AddRectFilled(start, end, IM_COL32(25, 25, 25, 200));
		window->DrawList->AddText(start + ImVec2(3.0f, 2.5f), Walnut::UI::Colors::Theme::text, text.data());
	}

	void TextClipped(std::string_view text, const ImRect& bb)
	{
		ImGuiContext& g = *GImGui;
		ImVec2 size = CalcTextSize(text.data(), NULL, true);

		if (bb.GetWidth() <= 1.0f) {
			return;
		}

		ImRect textPixelClipBB(bb.Min.x + g.Style.FramePadding.x, bb.Min.y + g.Style.FramePadding.y, bb.Max.x - g.Style.FramePadding.x, bb.Max.y);
		ImRect textEllipsisClipBB = textPixelClipBB;

		bool clipped = (textEllipsisClipBB.Min.x + size.x) > textPixelClipBB.Max.x;

		RenderTextEllipsis(g.CurrentWindow->DrawList, textEllipsisClipBB.Min, textEllipsisClipBB.Max, textPixelClipBB.Max.x, bb.Max.x - 1.0f, text.data(), NULL, &size);
	}

	void CenterText(std::string_view text)
	{
		ImGuiStyle& style = GetStyle();

		float actualSize = CalcTextSize(text.data()).x;
		float avail = GetContentRegionAvail().x;

		float off = (avail - actualSize) * 0.5f;
		if (off > 0.0f) {
			SetCursorPosX(GetCursorPosX() + off);
		}
	}

	void CenterTextHorizontalToWindow(std::string_view text)
	{
		ImGuiStyle& style = GetStyle();

		float actualSize = CalcTextSize(text.data()).x;
		float width = GetContentRegionMax().x;

		float off = (width - actualSize) * 0.5f;
		if (off > 0.0f) {
			SetCursorPosX(off);
		}
	}

	bool Selection(const std::vector<std::string>& items, size_t& index)
	{
		size_t prevItem = index;
		for (size_t i = 0; i < items.size(); i++) {
			const bool selected = index == i;
			if (Selectable(items[i].c_str(), selected)) {
				index = i;
			}
			if (selected) {
				SetItemDefaultFocus();
			}
		}
		return prevItem != index;
	}

	void BeginRedButtons()
	{
		ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(150, 0, 0, 200));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(150, 0, 0, 255));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(150, 0, 0, 150));
	}

	void EndRedButtons()
	{
		ImGui::PopStyleColor(3);
	}
}