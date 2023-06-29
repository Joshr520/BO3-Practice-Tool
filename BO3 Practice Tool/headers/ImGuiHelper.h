#pragma once

#include <string>
#include <vector>
#include <queue>
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"

#include "ListDefs.h"

#define COLOR_RED IM_COL32(170, 0, 0, 255)
#define COLOR_ORANGE IM_COL32(255, 128, 0, 255)
#define COLOR_GREEN IM_COL32(0, 128, 0, 255)
#define COLOR_BLUE IM_COL32(25, 100, 128, 255)
#define COLOR_BLUE_WEAK IM_COL32(25, 100, 128, 100)
#define COLOR_LIGHT_BLUE IM_COL32(0, 200, 200, 200)
#define COLOR_PINK IM_COL32(255, 105, 180, 255)
#define COLOR_GREY IM_COL32(128, 128, 128, 128)
#define COLOR_WHITE IM_COL32(255, 255, 255, 255)
#define COLOR_TRANSPARENT IM_COL32(0, 0, 0, 0)

namespace ImGuiHelper {
	
	struct SelectionData {
		std::vector<std::string> m_Items;
	};

	class MultiSpanSelection {
	public:
		MultiSpanSelection() : m_Selections({ 0 }), m_Changed(false), m_Index(0) { }
		MultiSpanSelection(SelectionData selection) : m_Changed(false), m_Index(0) { m_Selections.emplace_back(selection); }
		MultiSpanSelection(std::vector<SelectionData> selections) : m_Selections(selections), m_Changed(false), m_Index(0) {}

		void Render(int comboIndex);
		void SetIndex(int index) { m_Index = index; }
		void AddSelection(std::vector<std::string> selection) { m_Selections.emplace_back(selection); }
		void Reset() { m_Selections.clear(); };

		bool GetChanged() { return std::exchange(m_Changed, false); }
		int GetIndex() const { return m_Index; }
		int GetArrIndex() const;
		std::pair<int, int> GetMultiIndex() const;
		std::string GetItemAtIndex() const;
	private:
		std::vector<SelectionData> m_Selections;
		bool m_Changed;
		int m_Index;
	};

	struct Selection {
		static bool Render(const std::vector<std::string>& items, int& index);
		static bool RenderBGB(const std::vector<BGB>& items, int& index);
		static bool RenderBGBPreset(const std::vector<BGBPreset>& items, int& index);
		static bool RenderWeaponLoadout(const std::vector<MenuWeaponPreset>& items, int& index);
	};

	struct TextFont {
		static void Render(std::string_view text, ImFont* font, bool push = 1, bool pop = 1);
		static void RenderWrapped(std::string_view text, ImFont* font, bool push = 1, bool pop = 1);
	};

	typedef int PopupStates;

	enum PopupStates_ {
		None,
		Open,
		Update,
		UpdateFailed,
		InjectFailed,
		JoinDiscord,
		DirectoryError,
		GumPresetCreation,
		GumPresetError,
		WeaponLoadoutCreation,
		WeaponLoadoutError,
		AutosplitPresetCreation,
		AutosplitPresetError,
		AutosplitMapError
	};

	struct PopupWrapper {
	public:
		static PopupStates GetPrepState() { return s_PrepState; }
		static PopupStates GetOpenState() { return s_OpenState; }

		static void PrepPopup(PopupStates state);
		static void OpenPopup(PopupStates state);
		static void ClosePopup();
	private:
		static PopupStates s_PrepState;
		static PopupStates s_OpenState;
		static std::queue<PopupStates> s_StateQueue;
	};

	struct ButtonWrapper {
		static bool RenderToggle(std::string_view name, const ImVec2& size, bool active, bool displayOnly = false, const ImU32& colorOn = COLOR_GREEN, const ImU32& colorOff = COLOR_RED);
		static bool RenderToggleOut(std::string_view name, const ImVec2& size, bool* active, bool displayOnly = false, const ImU32& colorOn = COLOR_GREEN, const ImU32& colorOff = COLOR_RED);
		static bool RenderSingle(std::string_view name, const ImVec2& size, bool displayOnly = false, const ImU32& color = COLOR_BLUE);
		static void RenderFake(std::string_view name, const ImVec2& size, const ImU32& color = COLOR_TRANSPARENT);
	};

	struct ListBoxWrapper {
		static bool Render(std::string_view name, const std::vector<std::string>& items, int& index, const ImVec2& size);
	};

}