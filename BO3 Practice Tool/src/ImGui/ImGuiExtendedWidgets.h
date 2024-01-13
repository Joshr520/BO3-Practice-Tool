#pragma once

#include "ImGuiUtils.h"
#include "Embed/IcoMoon.h"
#include "Walnut/ImGui/ImGuiTheme.h"

#include "imgui_internal.h"

#include <vector>

namespace ImGui
{
	class TextEditSelectable {
	public:
		struct TextEditResponse {
			bool TextChanged;
			bool ItemHovered;
			std::pair<std::string, std::string> OldAndNewText;
		};

		TextEditSelectable(std::string_view id, std::string_view text) : m_ID(std::format("{}", id)), m_Text(text), m_EditingText(text) {}
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

	struct CreateFileResponse {
		bool Created;
		TextEditSelectable NewFile;
	};

	class FileEditor {
	public:
		FileEditor(std::string_view id, const std::vector<TextEditSelectable>& files) : m_Files(files), m_PrefixID(id) {}
		FileEditor(std::string_view id, const std::vector<std::string>& files);

		void Render();

		void ClearSelections();

		void SetFileExt(std::string_view ext) { m_FileExt = ext; }
		void SetDirectory(std::string_view directory) { m_Directory = directory; }

		void SetCallbackNew(std::function<void(std::string_view)> callback) { m_CallbackNew = callback; }
		void SetCallbackDelete(std::function<void(std::string_view)> callback) { m_CallbackDelete = callback; }
		void SetCallbackLastSelectedChanged(std::function<void(size_t)> callback) { m_CallbackLastSelectedChanged = callback; }
		void SetCallbackEdited(std::function<void(std::string_view, std::string_view)> callback) { m_CallbackEdited = callback; }

		CreateFileResponse CreateEmptyFile(std::string_view name = "");
		void AddFile(const TextEditSelectable& newFile);
		void RemoveFiles();
		bool NameFile(TextEditSelectable& file, std::string_view oldName, std::string_view newName);
		bool CheckFileExists(const TextEditSelectable& file);

		void ReformatFiles();
		std::string GetLatestFormattedID();
	private:
		std::vector<TextEditSelectable> m_Files;
		std::vector<TextEditSelectable> m_Clipboard = { };
		std::vector<size_t> m_SelectedFiles = { };

		std::string m_PrefixID;
		std::string m_FileExt = ".txt";
		std::string m_Directory = "./";

		bool m_RightClickMenuItem = false;
		bool m_RightClickMenuWindow = false;

		size_t m_StartSelection = -1;
		size_t m_LastSelected = 0;

		std::function<void(size_t index)> m_CallbackLastSelectedChanged = nullptr;
		std::function<void(std::string_view name)> m_CallbackNew = nullptr;
		std::function<void(std::string_view name)> m_CallbackDelete = nullptr;
		std::function<void(std::string_view oldName, std::string_view newName)> m_CallbackEdited = nullptr;
	};

	struct ImageSelectionResponse {
		bool Hovered;
		bool Pressed;
	};

	class ImageSelection {
	public:
		ImageSelection() { }
		ImageSelection(int8_t selected) : m_Selected(selected) { }

		ImageSelectionResponse Render(int8_t index, ImTextureID textureID, const ImVec2& size, bool toggle = false);

		int8_t GetSelected() const { return m_Selected; }

		void SetSelected(int8_t selected) { m_Selected = selected; }
		void Deselect() { m_Selected = -1; }
	private:
		int8_t m_Selected = 0;
	};

	class ImageMultiSelection {
	public:

		ImageSelectionResponse Render(size_t index, ImTextureID textureID, const ImVec2& size);

		void SetMaxSelections(size_t maxSelections) { m_MaxSelections = maxSelections; }
		void SetSelected(size_t num) { m_SelectedItems.emplace_back(num); }

		const std::vector<size_t>& GetSelected() const { return m_SelectedItems; }
		void Deselect(size_t value);
		void ClearAll() { m_SelectedItems.clear(); }
	private:
		size_t m_MaxSelections = 5;
		std::vector<size_t> m_SelectedItems = { };
	};

	struct SelectionData {
		std::vector<std::string> Selection;
	};

	class MultiSelection {
	public:
		MultiSelection() { }
		MultiSelection(std::vector<SelectionData> selections) { m_Selections = selections; }

		bool Render(bool autoInc = true);
		void SetIndex(size_t index) { m_DisplayIndex = index; }
		void Clear() { m_Selections.clear(); m_DisplayIndex = 0; m_SelectionIndex = 0; m_ItemIndex = 0; };

		void AddSelection(SelectionData selection) { m_Selections.push_back(selection); }

		std::string GetSelectedItem() const { return m_Selections[m_SelectionIndex].Selection[m_ItemIndex]; }
		size_t GetSelectionIndex() const { return m_SelectionIndex; }
		size_t GetItemIndex() const { return m_ItemIndex; }
	private:
		std::vector<SelectionData> m_Selections = { };
		size_t m_DisplayIndex = 0;
		size_t m_SelectionIndex = 0;
		size_t m_ItemIndex = 0;
	};

	void HelpMarker(std::string_view text);
	void ItemTooltip(std::string_view text);
	bool Splitter(const char* label, bool split_vertically, float thickness, float* size1, float* size2, float min_size1, float min_size2, float splitter_long_axis_size = -1.0f);
	bool BackButton();
	bool ActiveButton(std::string_view label, bool* active, ImVec2 size = { 0, 0 }, ImU32 colorOn = IM_COL32(0, 150, 0, 200), ImU32 colorOff = IM_COL32(200, 0, 0, 200));
	bool ToggleButton(std::string_view id, std::string_view name, bool* value);
}