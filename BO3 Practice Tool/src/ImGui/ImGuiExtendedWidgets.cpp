#include "ImGuiExtendedWidgets.h"
#include "imgui_internal.h"

#include "PracticeTool/Core/Log.h"
#include "PracticeTool/UI/Notifications.h"

#include <fstream>
#include <filesystem>

const std::string illegalTextNoti = "Name cannot include any of the following characters -  \'<>:\"/\\|?*\'"
"\nAny such characters have been removed.";

std::string RemoveIllegalChars(std::string_view oldName, std::string_view newName)
{
	const std::string illegal = "<>:\"/\\|?*";
	std::string newText = newName.data();

	newText.erase(std::remove_if(newText.begin(), newText.end(), [&](char c) {
		return illegal.find(c) != std::string::npos;
		}), newText.end());

	if (newText != newName) {
		BO3PracticeTool::InfoNotification::AddInfoNoti("File Naming Error", illegalTextNoti);
		if (newText.empty()) {
			BO3PracticeTool::InfoNotification::AddInfoNoti("File Naming Error", "Resulting name was empty after illegal characters were removed.");
		}
	}
	if (newText.empty()) {
		newText = oldName;
	}

	return newText;
}

namespace ImGui
{
#define GET_FILEPATH(filename) std::format("{}\\{}{}", m_Directory, filename, m_FileExt)

	FileEditor::FileEditor(std::string_view id, const std::vector<std::string>& files)
		: m_PrefixID(id)
	{
		std::vector<TextEditSelectable> textFiles;
		for (size_t i = 0; i < files.size(); i++) {
			const std::string& file = files[i];
			textFiles.emplace_back(TextEditSelectable(std::format("##{}_{}", id, i), file));
		}
		m_Files = textFiles;
	}

	void FileEditor::Render()
	{
		bool anyHovered = false;
		bool anyEditing = false;
		bool popupItemHovered = false;
		bool popupWindowHovered = false;
		bool windowActive = IsWindowFocused();
		PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

		for (size_t i = 0; i < m_Files.size(); i++) {
			size_t index = i;
			TextEditSelectable& file = m_Files[i];
			TextEditSelectable::TextEditResponse returnValue = file.Render();
			const std::string oldName = returnValue.OldAndNewText.first;
			const std::string newName = returnValue.OldAndNewText.second;

			if (returnValue.TextChanged) {
				if (NameFile(file, oldName, RemoveIllegalChars(oldName, newName)) && m_CallbackEdited) {
					m_CallbackEdited(oldName, newName);
				}
				else if (newName.empty()) {
					BO3PracticeTool::InfoNotification::AddInfoNoti("File Naming Error", "Name can't be empty.");
				}
				std::sort(m_Files.begin(), m_Files.end());
			}
			const bool selected = file.GetSelected();
			const bool beingEdited = file.GetBeingEdited();
			if (beingEdited) {
				anyEditing = true;
			}
			// Process left and right click events
			if (returnValue.ItemHovered) {
				if (IsMouseClicked(ImGuiMouseButton_Left)) {
					if (m_StartSelection < 0) {
						m_StartSelection = index;
						m_LastSelected = index;
						if (m_CallbackLastSelectedChanged) {
							m_CallbackLastSelectedChanged(index);
						}
						m_SelectedFiles = { index };
						file.SetSelected(true);
					}
					else if (IsKeyDown(ImGuiKey_ModShift)) {
						if (!beingEdited && m_StartSelection == index) {
							ClearSelections();
							file.SetSelected(true);
							file.SetBeingEdited(true);
						}
						else {
							ClearSelections();
							int step = (m_StartSelection <= index) ? 1 : -1;
							for (size_t fileIndex = m_StartSelection; (m_StartSelection <= index) ? fileIndex <= index : fileIndex >= index; fileIndex += step) {
								TextEditSelectable& tempFile = m_Files[fileIndex];

								tempFile.SetSelected(true);
								m_SelectedFiles.emplace_back(fileIndex);
							}
						}
					}
					else if (IsKeyDown(ImGuiKey_ModCtrl)) {
						m_StartSelection = index;
						m_LastSelected = index;
						if (m_CallbackLastSelectedChanged) {
							m_CallbackLastSelectedChanged(index);
						}
						file.SetSelected(!selected);
						if (selected) {
							m_SelectedFiles.erase(std::find(m_SelectedFiles.begin(), m_SelectedFiles.end(), index));
						}
						else {
							m_SelectedFiles.emplace_back(index);
						}
					}
					else {
						ClearSelections();
						m_StartSelection = index;
						m_LastSelected = index;
						if (m_CallbackLastSelectedChanged) {
							m_CallbackLastSelectedChanged(index);
						}
						m_SelectedFiles = { index };
						if (selected) {
							file.SetBeingEdited(true);
						}
						else {
							file.SetSelected(true);
						}
					}

					m_RightClickMenuItem = false;
					m_RightClickMenuWindow = false;
				}
				else if (IsMouseReleased(ImGuiMouseButton_Right)) {
					if (IsKeyDown(ImGuiKey_ModShift)) {
						ClearSelections();
						size_t step = (m_StartSelection <= index) ? 1 : -1;
						for (size_t fileIndex = m_StartSelection; (m_StartSelection <= index) ? fileIndex <= index : fileIndex >= index; fileIndex += step) {
							TextEditSelectable& tempFile = m_Files[fileIndex];

							tempFile.SetSelected(true);
							m_SelectedFiles.emplace_back(fileIndex);
						}
					}
					else if (IsKeyDown(ImGuiKey_ModCtrl)) {
						m_StartSelection = index;
						m_LastSelected = index;
						if (m_CallbackLastSelectedChanged) {
							m_CallbackLastSelectedChanged(index);
						}
						if (std::find(m_SelectedFiles.begin(), m_SelectedFiles.end(), index) == m_SelectedFiles.end()) {
							m_SelectedFiles.emplace_back(index);
						}
					}
					else {
						ClearSelections();
						m_StartSelection = index;
						m_LastSelected = index;
						if (m_CallbackLastSelectedChanged) {
							m_CallbackLastSelectedChanged(index);
						}
						m_SelectedFiles.emplace_back(index);
					}

					file.SetSelected(true);
					m_RightClickMenuItem = true;
					m_RightClickMenuWindow = false;
					OpenPopup("##Right Click Menu Item");
				}

				anyHovered = true;
			}
		}
		PopStyleVar();
		// Render right click menu item if open
		if (m_RightClickMenuItem) {
			if (BeginPopup("##Right Click Menu Item")) {
				popupItemHovered = IsWindowHovered();

				ImGuiSelectableFlags flags = m_Clipboard.empty() ? ImGuiSelectableFlags_Disabled : ImGuiSelectableFlags_None;

				if (Selectable("Copy")) {
					m_Clipboard.clear();
					for (const size_t& index : m_SelectedFiles) {
						m_Clipboard.emplace_back(m_Files[index]);
					}
				}
				if (Selectable("Paste", false, flags)) {
					for (const TextEditSelectable& file : m_Clipboard) {
						CreateFileResponse response = CreateEmptyFile(file.GetText());
						if (response.Created) {
							TextEditSelectable newFile = response.NewFile;
							AddFile(newFile);
						}
					}
					ReformatFiles();
					ClearSelections();
					std::sort(m_Files.begin(), m_Files.end());
				}
				if (Selectable("Duplicate")) {
					const TextEditSelectable& file = m_Files[m_StartSelection];
					CreateFileResponse response = CreateEmptyFile(file.GetText());
					if (response.Created) {
						TextEditSelectable newFile = response.NewFile;
						AddFile(newFile);
						ReformatFiles();
						ClearSelections();
						std::sort(m_Files.begin(), m_Files.end());
					}
				}
				if (Selectable("Rename")) {
					m_Files[m_StartSelection].SetBeingEdited(true);
				}
				if (Selectable("Delete")) {
					if (m_SelectedFiles.size()) {
						RemoveFiles();
						ReformatFiles();
						m_SelectedFiles.clear();
						std::sort(m_Files.begin(), m_Files.end());
					}
				}

				EndPopup();
			}
		}
		// Look for right click while not hovering an item and mouse is inside window
		if (!popupItemHovered && !anyHovered && IsMouseReleased(ImGuiMouseButton_Right)) {
			const ImVec2 pos = GetWindowPos();
			const ImVec2 size = GetWindowSize();

			if (IsMouseHoveringRect(pos, pos + size)) {
				m_RightClickMenuWindow = true;
				OpenPopup("##Right Click Menu Window");
			}
		}
		// Render right click menu window if open
		if (m_RightClickMenuWindow) {
			if (BeginPopup("##Right Click Menu Window")) {
				popupWindowHovered = IsWindowHovered();

				ImGuiSelectableFlags flags = m_Clipboard.empty() ? ImGuiSelectableFlags_Disabled : ImGuiSelectableFlags_None;

				if (Selectable("New")) {
					CreateFileResponse response = CreateEmptyFile(GetLatestFormattedID());
					if (response.Created) {
						TextEditSelectable newFile = response.NewFile;
						AddFile(newFile);
						ReformatFiles();
						ClearSelections();
						std::sort(m_Files.begin(), m_Files.end());
					}
				}
				if (Selectable("Paste", false, flags)) {
					for (const TextEditSelectable& file : m_Clipboard) {
						CreateFileResponse response = CreateEmptyFile(file.GetText());
						if (response.Created) {
							TextEditSelectable newFile = response.NewFile;
							AddFile(newFile);
						}
					}
					ReformatFiles();
					ClearSelections();
					std::sort(m_Files.begin(), m_Files.end());
				}

				EndPopup();
			}
		}
		// Ctrl+C & Ctrl+V copy/paste macros
		// Ctrl+N & Ctrl+A new/delete macros
		// Del macro
		if (!anyEditing && windowActive) {
			if (IsKeyDown(ImGuiKey_ModCtrl)) {
				if (IsKeyPressed(ImGuiKey_C, false)) {
					if (m_SelectedFiles.size() > 0) {
						m_Clipboard.clear();
						for (const size_t& index : m_SelectedFiles) {
							m_Clipboard.emplace_back(m_Files[index]);
						}
					}
				}
				else if (IsKeyPressed(ImGuiKey_V, false)) {
					if (m_Clipboard.size() > 0) {
						for (const TextEditSelectable& file : m_Clipboard) {
							CreateFileResponse response = CreateEmptyFile(file.GetText());
							if (response.Created) {
								TextEditSelectable newFile = response.NewFile;
								AddFile(newFile);
							}
						}
						ReformatFiles();
						ClearSelections();
						std::sort(m_Files.begin(), m_Files.end());
					}
				}
				else if (IsKeyPressed(ImGuiKey_N, false)) {
					CreateFileResponse response = CreateEmptyFile(GetLatestFormattedID());
					if (response.Created) {
						TextEditSelectable newFile = response.NewFile;
						AddFile(newFile);
						ReformatFiles();
						ClearSelections();
						std::sort(m_Files.begin(), m_Files.end());
					}
				}
				else if (IsKeyPressed(ImGuiKey_A, false)) {
					m_StartSelection = 0;
					m_SelectedFiles.clear();
					for (size_t i = 0; i < m_Files.size(); i++) {
						TextEditSelectable& file = m_Files[i];
						file.SetSelected(true);
						m_SelectedFiles.emplace_back(i);
					}
				}
			}
			else if (IsKeyPressed(ImGuiKey_Delete, false)) {
				if (m_SelectedFiles.size()) {
					RemoveFiles();
					ReformatFiles();
					std::sort(m_Files.begin(), m_Files.end());
					m_SelectedFiles.clear();
				}
			}
		}
		// Reset start selection if none are hovered and mouse is clicked elsewhere
		if (m_StartSelection >= 0 && !anyHovered && !popupItemHovered && !popupWindowHovered && (IsMouseClicked(ImGuiMouseButton_Left) || IsMouseClicked(ImGuiMouseButton_Right))) {
			m_StartSelection = -1;
			m_RightClickMenuWindow = false;
			m_RightClickMenuItem = false;
			ClearSelections();
		}
	}

	void FileEditor::ClearSelections()
	{
		for (const size_t& index : m_SelectedFiles) {
			m_Files[index].SetSelected(false);
		}
		m_SelectedFiles.clear();
	}

	CreateFileResponse FileEditor::CreateEmptyFile(std::string_view name)
	{
		TextEditSelectable file;
		bool valid = true;

		file.SetID(std::format("##{}{}", m_PrefixID, m_Files.size()));
		if (NameFile(file, "", name) && !std::ofstream(GET_FILEPATH(file.GetText()), std::ios::app).is_open()) {
			char error[128];
			strerror_s(error, 128, errno);
			LOG_ERROR("Error opening file {}: {}", file.GetText(), error);
			BO3PracticeTool::InfoNotification::AddInfoNoti("File Opening Error", std::format("An error occurred while opening file {}.", file.GetText()));
			valid = false;
		}
		return { valid, file };
	}

	void FileEditor::AddFile(const TextEditSelectable& newFile)
	{
		m_Files.emplace_back(newFile);
		if (m_CallbackNew) {
			m_CallbackNew(newFile.GetText());
		}
	}

	void FileEditor::RemoveFiles()
	{
		std::vector<TextEditSelectable> filesToDelete;
		for (const size_t& index : m_SelectedFiles) {
			filesToDelete.emplace_back(m_Files[index]);
		}
		for (const TextEditSelectable& fileToDelete : filesToDelete) {
			if (m_CallbackDelete) {
				m_CallbackDelete(fileToDelete.GetText());
			}
			m_Files.erase(std::find(m_Files.begin(), m_Files.end(), fileToDelete));
			std::string name = fileToDelete.GetText();
			if (std::filesystem::exists(GET_FILEPATH(name))) {
				try {
					std::filesystem::remove(GET_FILEPATH(name));
				}
				catch (const std::filesystem::filesystem_error& ex) {
					LOG_ERROR("Error deleting file {}: {}", name, ex.what());
					BO3PracticeTool::InfoNotification::AddInfoNoti("File Deleting Error", std::format("An error occurred while Deleting file {}.", name));
				}
			}
		}
	}

	bool FileEditor::NameFile(TextEditSelectable& file, std::string_view oldName, std::string_view newName)
	{
		if (oldName == newName) {
			file.SetText(oldName);
			return false;
		}
		file.SetText(newName);
		int numFails = 1;
		while (CheckFileExists(file)) {
			if (numFails <= 1) {
				file.SetText(std::format("{} - Copy", newName));
			}
			else {
				file.SetText(std::format("{} - Copy ({})", newName, numFails));
			}
			numFails++;
		}

		if (!std::filesystem::exists(GET_FILEPATH(oldName))) {
			return true;
		}

		try {
			std::filesystem::rename(GET_FILEPATH(oldName), GET_FILEPATH(newName));
		}
		catch (const std::filesystem::filesystem_error& ex) {
			LOG_ERROR("Error renaming file {} to {}: {}", oldName, newName, ex.what());
			BO3PracticeTool::InfoNotification::AddInfoNoti("File Renaming Error", std::format("An error occurred while renaming file {}.", oldName));
		}

		return true;
	}

	bool FileEditor::CheckFileExists(const TextEditSelectable& file)
	{
		for (const TextEditSelectable& internalFile : m_Files) {
			if (internalFile == file) {
				continue;
			}
			if (internalFile.GetText() == file.GetText()) {
				return true;
			}
		}
		return false;
	}

	void FileEditor::ReformatFiles()
	{
		for (size_t i = 0; i < m_Files.size(); i++) {
			TextEditSelectable& file = m_Files[i];
			file.SetID(std::format("##{} {}", m_PrefixID, i));
		}
	}

	std::string FileEditor::GetLatestFormattedID()
	{
		return std::format("{} {}", m_PrefixID, m_Files.size() + 1);
	}



	ImageSelectionResponse ImageSelection::Render(size_t index, ImTextureID textureID, const ImVec2& size, bool toggle)
	{
		ImGuiContext& g = *GImGui;
		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems) {
			return { false, false };
		}

		PushID((void*)(intptr_t)textureID);
		const ImGuiID id = window->GetID("#image");
		PopID();

		const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size);
		ItemSize(bb);
		if (!ItemAdd(bb, id)) {
			return { false, false };
		}

		bool hovered, held;
		bool pressed = ButtonBehavior(bb, id, &hovered, &held);

		const ImVec2& padding = g.Style.FramePadding;
		const ImU32 col = GetColorU32((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
		RenderNavHighlight(bb, id);
		RenderFrame(bb.Min, bb.Max, col, true, ImClamp((float)ImMin(padding.x, padding.y), 0.0f, 0.0f));

		window->DrawList->AddRectFilled(bb.Min, bb.Max, IM_COL32(0, 0, 0, 0));
		window->DrawList->AddImage(textureID, bb.Min, bb.Max);
		if (m_Selected == index) {
			window->DrawList->AddRect(bb.Min, bb.Max, IM_COL32(255, 70, 0, 255));
		}

		if (pressed) {
			if (m_Selected == index && toggle) {
				m_Selected = -1;
			}
			else {
				m_Selected = index;
			}
		}

		return { hovered, pressed };
	}

	ImageSelectionResponse ImageMultiSelection::Render(size_t index, ImTextureID textureID, const ImVec2& size)
	{
		ImGuiContext& g = *GImGui;
		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems) {
			return { false, false };
		}

		PushID((void*)(intptr_t)textureID);
		const ImGuiID id = window->GetID("#image");
		PopID();

		const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size);
		ItemSize(bb);
		if (!ItemAdd(bb, id)) {
			return { false, false };
		}

		bool hovered, held;
		bool pressed = ButtonBehavior(bb, id, &hovered, &held);
		auto it = std::find(m_SelectedItems.begin(), m_SelectedItems.end(), index);
		bool selected = it != m_SelectedItems.end();

		const ImVec2& padding = g.Style.FramePadding;
		const ImU32 col = GetColorU32((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
		RenderNavHighlight(bb, id);
		RenderFrame(bb.Min, bb.Max, col, true, ImClamp((float)ImMin(padding.x, padding.y), 0.0f, 0.0f));

		window->DrawList->AddRectFilled(bb.Min, bb.Max, IM_COL32(0, 0, 0, 0));
		window->DrawList->AddImage(textureID, bb.Min, bb.Max);
		if (selected) {
			window->DrawList->AddRect(bb.Min, bb.Max, IM_COL32(255, 70, 0, 255));
			std::string numSelected = std::to_string(std::distance(m_SelectedItems.begin(), it) + 1);
			ImVec2 textSize = CalcTextSize(numSelected.c_str());
			ImVec2 textStart(bb.Max.x - textSize.x - 10.0f, bb.Min.y + 5.0f);
			TextBackground(textStart, numSelected);
		}

		if (pressed) {
			if (selected) {
				m_SelectedItems.erase(it);
			}
			else {
				if (m_SelectedItems.size() == m_MaxSelections) {
					m_SelectedItems.erase(m_SelectedItems.begin());
				}
				m_SelectedItems.emplace_back(index);
			}
		}

		return { hovered, pressed };
	}
}