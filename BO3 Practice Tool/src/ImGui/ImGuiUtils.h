#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
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

	typedef enum class PopupStates : uint8_t {
		None,
		Open,
		About,
		Update,
		UpdateFailed,
		InjectFailed,
		JoinDiscord,
		DownloadCompilerFiles,
		DirectoryError,
		GumPresetCreation,
		GumPresetError,
		WeaponLoadoutCreation,
		WeaponLoadoutError,
		AutosplitPresetCreation,
		AutosplitPresetError,
		AutosplitMapError
	} PopupState;

	class PopupWrapper {
	public:
		static PopupState GetPrepState() { return s_PrepState; }
		static PopupState GetOpenState() { return s_OpenState; }

		static void PrepPopup(PopupState state);
		static void OpenPopup(PopupState state);
		static void ClosePopup();
	private:
		static PopupState s_PrepState;
		static PopupState s_OpenState;
		static std::queue<PopupState> s_StateQueue;
	};

	class TextURL {
	public:
		static void Render(std::string_view text, std::string_view url, bool continueBefore = 1, bool continueAfter = 1);
	private:
		static void UnderlineText(ImColor color);
	};

	class Text {
	public:
		static void CenterText(std::string_view text);
	};

	class Selection {
	public:
		static bool Render(const std::vector<std::string>& items, int& index);
	};

	class TextEditSelectable {
	public:
		struct TextEditResponse {
			bool TextChanged;
			bool ItemHovered;
			std::pair<std::string, std::string> OldAndNewText;
		};

		TextEditSelectable(std::string_view id, std::string_view text) : m_ID(std::format("{}", id)), m_Text(text), m_EditingText(text){}
		TextEditSelectable() : m_ID(""), m_Text(""), m_EditingText("") {}

		TextEditResponse Render();

		bool GetSelected() const { return m_Selected; }
		bool GetBeingEdited() const { return m_BeingEdited; }
		std::string GetTextID() const { return m_ID; }
		std::string GetText() const { return m_Text; }

		void SetSelected(bool value) { m_Selected = value; }
		void SetBeingEdited(bool value) { m_BeingEdited = value; }
		void SetID(std::string_view id) { m_ID = id; }
		void SetText(std::string_view text) { m_Text = text; m_EditingText = text; }

		bool operator<(const TextEditSelectable& text) const { return strnatcmp(m_Text.c_str(), text.m_Text.c_str()) < 0; }
		bool operator==(const TextEditSelectable& text) const { return m_Text == text.m_Text && m_ID == text.m_ID; }
	private:
		bool m_Selected = false;
		bool m_BeingEdited = false;
		bool m_FirstEdit = false;
		std::string m_ID;
		std::string m_Text;
		std::string m_EditingText;
	};
}