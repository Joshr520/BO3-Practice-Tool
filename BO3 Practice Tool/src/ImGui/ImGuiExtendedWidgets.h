#pragma once

#include "ImGuiUtils.h"
#include "Embed/IcoMoon.h"

#include "imgui_internal.h"

#include <vector>

namespace ImGui
{
	struct CreateFileResponse {
		bool Created;
		TextEditSelectable NewFile;
	};

	class FileEditor {
	public:
		FileEditor(std::string_view id, const std::vector<TextEditSelectable>& files) : m_Files(files), m_PrefixID(id) {}
		FileEditor(std::string_view id, const std::vector<std::string>& files);

		void RenderFiles();
		
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
		ImageSelectionResponse RenderImageSelection(size_t index, ImTextureID textureID, const ImVec2& size);

		size_t GetSelected() const { return m_Selected; }
	private:
		size_t m_Selected = 0;
	};

	inline void HelpMarker(std::string_view text)
	{
		TextDisabled(FONT_QUESTION);
		if (IsItemHovered()) {
			BeginTooltip();
			PushTextWrapPos(GetFontSize() * 35.0f);
			TextWrapped(text.data());
			PopTextWrapPos();
			EndTooltip();
		}
	}

	inline bool Splitter(const char* label, bool split_vertically, float thickness, float* size1, float* size2, float min_size1, float min_size2, float splitter_long_axis_size = -1.0f)
	{
		using namespace ImGui;
		ImGuiContext& g = *GImGui;
		ImGuiWindow* window = g.CurrentWindow;
		ImGuiID id = window->GetID(label);
		ImRect bb;
		bb.Min = window->DC.CursorPos + (split_vertically ? ImVec2(*size1, 0.0f) : ImVec2(0.0f, *size1));
		bb.Max = bb.Min + CalcItemSize(split_vertically ? ImVec2(thickness, splitter_long_axis_size) : ImVec2(splitter_long_axis_size, thickness), 0.0f, 0.0f);
		return SplitterBehavior(bb, id, split_vertically ? ImGuiAxis_X : ImGuiAxis_Y, size1, size2, min_size1, min_size2, 0.0f);
	}
}