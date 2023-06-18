#pragma once

#include <string>
#include <vector>

#include "imgui.h"

#define COLOR_RED IM_COL32(170, 0, 0, 255)
#define COLOR_GREEN IM_COL32(0, 128, 0, 255)
#define COLOR_BLUE IM_COL32(25, 100, 128, 255)
#define COLOR_BLUE_WEAK IM_COL32(25, 100, 128, 100)
#define COLOR_PINK IM_COL32(255, 105, 180, 255)
#define COLOR_GREY IM_COL32(128, 128, 128, 128)
#define COLOR_TRANSPARENT IM_COL32(0, 0, 0, 0)

namespace ImGuiHelper {
	
	struct SelectionData {
		std::vector<std::string> m_Items;
	};

	class MultiSpanSelection {
	public:
		MultiSpanSelection(SelectionData selection) : m_Changed(false), m_Index(0) { m_Selections.emplace_back(selection); }
		MultiSpanSelection(std::vector<SelectionData> selections) : m_Selections(selections), m_Changed(false), m_Index(0) {}

		void Render(int comboIndex);
		void SetIndex(int index) { m_Index = index; }

		bool GetChanged() { return std::exchange(m_Changed, false); }
		int GetIndex() const { return m_Index; }
	private:
		std::vector<SelectionData> m_Selections;
		bool m_Changed;
		int m_Index;

	};

	struct TextFont {
		static void Render(std::string_view text, ImFont* font, bool push = 1, bool pop = 1);
		static void RenderWrapped(std::string_view text, ImFont* font, bool push = 1, bool pop = 1);
	};

	typedef int PopupStates;

	enum PopupStates_ {
		ShowUpdate = 1,
		ShowUpdateFailed,
		ShowInjectFailed,
		ShowJoinDiscord
	};

	struct PopupWrapper {
	public:
		static PopupStates GetState() { return s_State; }
		static bool IsStateSet(PopupStates state) { return (s_State & state) == state; }

		static void SetState(PopupStates state) { s_State |= state; }
		static void UnsetState(PopupStates state) { s_State &= ~state; }
	private:
		static PopupStates s_State;
	};

	struct ButtonWrapper {
		static bool RenderToggle(std::string_view name, const ImVec2& size, bool active, bool displayOnly = false, const ImU32& colorOn = COLOR_GREEN, const ImU32& colorOff = COLOR_RED);
		static bool RenderToggleOut(std::string_view name, const ImVec2& size, bool* active, bool displayOnly = false, const ImU32& colorOn = COLOR_GREEN, const ImU32& colorOff = COLOR_RED);
		static bool RenderSingle(std::string_view name, const ImVec2& size, bool displayOnly = false, const ImU32& color = COLOR_BLUE);
		static void RenderFake(std::string_view name, const ImVec2& size, const ImU32& color = COLOR_TRANSPARENT);
	};

}