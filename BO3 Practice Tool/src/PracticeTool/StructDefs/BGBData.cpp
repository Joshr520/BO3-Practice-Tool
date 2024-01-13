#include "BGBData.h"

#include "PracticeTool/Core/Log.h"
#include "PracticeTool/LibHelpers/json.h"
#include "PracticeTool/LibHelpers/misc.h"
#include "PracticeTool/Core/Memory.h"
#include "PracticeTool/StructDefs/GameData.h"
#include "PracticeTool/UI/WindowBody.h"

#include <filesystem>
#include <fstream>

#define BGB_DIRECTORY "Settings\\BGB Presets\\"
#define BGB_FILENAME(name) std::format(BGB_DIRECTORY "{}.json", name)

namespace BO3PracticeTool
{
	std::vector<BGBPreset> BGBPreset::s_Presets;
	BGBPreset* BGBPreset::s_CurrentPreset = nullptr;

	void SafeEraseBGBPreset(std::vector<BGBPreset>& presets, std::string_view presetName)
	{
		presets.erase(std::remove_if(presets.begin(), presets.end(), [&](const BGBPreset& preset) {
			return preset.GetPresetName() == presetName;
			}), presets.end());
	}

	void  BGBPreset::SetBGB(size_t index, BGB* oldBGB, const BGB& newBGB)
	{
		for (BGB*& bgb : m_BGBs) {
			if (bgb->Name == newBGB.Name) {
				bgb = oldBGB;
				break;
			}
		}
		auto it = std::find(bgbs.begin(), bgbs.end(), newBGB);
		m_BGBs[index] = &*it;
	}

	void BGBPreset::SavePreset()
	{
		JSON json;
		std::ofstream file;
		bool writeToGame = Memory::injected && bgbPresetsActive;

		if (writeToGame) {
			file.open(practiceToolDirectory + "\\BGB Preset.txt");
		}

		rapidjson::Value& gums = json.AddArray(json.GetDocument(), "Gums");
		for (const BGB* bgb : m_BGBs) {
			rapidjson::Value gumValue;
			gumValue.SetString(rapidjson::StringRef(bgb->InternalName.c_str()));
			gums.PushBack(gumValue, json.GetAllocator());
			if (writeToGame) {
				file << bgb->InternalName << "\n";
			}
		}

		if (file.is_open()) {
			file.close();
		}

		json.SaveToFile(BGB_FILENAME(m_Name));
	}

	void BGBPreset::SavePreset(std::string_view name)
	{
		auto it = std::find(s_Presets.begin(), s_Presets.end(), name);
		if (it != s_Presets.end()) {
			it->SavePreset();
		}
	}

	void BGBPreset::WritePresetToGame(bool reset)
	{
		BGBPreset* currentPreset = BGBPreset::GetCurrentPreset();

		if (!currentPreset || reset) {
			std::ofstream(practiceToolDirectory + "\\BGB Preset.txt");
			return;
		}

		std::ofstream file(practiceToolDirectory + "\\BGB Preset.txt");

		for (size_t i = 0; i < 5; i++) {
			const BGB* bgb = currentPreset->GetBGB(i);
			file << bgb->InternalName << "\n";
		}

		file.close();
	}

	void BGBPreset::LoadBGBPresets()
	{
		LOG_DEBUG("Loading BGB presets");

		if (!CheckDirectory(BGB_DIRECTORY)) {
			return;
		}

		bgbFiles.SetFileExt(".json");
		bgbFiles.SetDirectory(BGB_DIRECTORY);

		for (const auto& directory : std::filesystem::directory_iterator(BGB_DIRECTORY)) {
			const std::string filename = directory.path().filename().stem().string();
			BGBPreset preset(filename);
			JSON json = JSON::FromFile(directory.path().string());
			const rapidjson::Document& document = json.GetDocument();

			if (!JSON::IsArray(document, "Gums")) {
				continue;
			}

			const rapidjson::Value& gumsArray = document["Gums"];
			bool valid = true;

			for (rapidjson::SizeType i = 0; i < gumsArray.Size(); i++) {
				if (gumsArray[i].IsString()) {
					auto it = std::find(bgbs.begin(), bgbs.end(), gumsArray[i].GetString());
					if (it != bgbs.end()) {
						BGB bgb = *it;
						preset.SetBGB(i, preset.GetBGB(i), bgb);
					}
					else {
						valid = false;
						break;
					}
				}
			}

			if (!valid) {
				continue;
			}

			ImGui::CreateFileResponse response = bgbFiles.CreateEmptyFile(filename);
			if (!response.Created) {
				continue;
			}
			s_Presets.emplace_back(preset);
			bgbFiles.AddFile(response.NewFile);
		}

		if (s_Presets.size()) {
			s_CurrentPreset = &s_Presets[0];
		}

		bgbFiles.ReformatFiles();
		bgbFiles.SetCallbackNew(AddBGBPreset);
		bgbFiles.SetCallbackDelete(DeleteBGBPreset);
		bgbFiles.SetCallbackLastSelectedChanged(BGBPresetIndexChanged);
		bgbFiles.SetCallbackEdited(EditedBGBPreset);
	}

	void BGBPreset::AddBGBPreset(std::string_view name)
	{
		if (!CheckDirectory(BGB_DIRECTORY)) {
			return;
		}
		LOG_DEBUG("BGB preset {} created", name);
		s_Presets.emplace_back(BGBPreset(name));
		s_CurrentPreset = &s_Presets[s_Presets.size() - 1];
		BGBPreset::SavePreset(name);
	}

	void BGBPreset::DeleteBGBPreset(std::string_view name)
	{
		if (s_CurrentPreset->m_Name == s_Presets[s_Presets.size() - 1].m_Name) {
			if (s_Presets.size() > 1) {
				s_CurrentPreset = &s_Presets[s_Presets.size() - 2];
			}
			else {
				s_CurrentPreset = nullptr;
			}
		}
		LOG_DEBUG("BGB preset {} deleted", name);
		SafeEraseBGBPreset(s_Presets, name);
	}

	void BGBPreset::EditedBGBPreset(std::string_view oldName, std::string_view newName)
	{
		auto it = std::find(s_Presets.begin(), s_Presets.end(), oldName);
		if (it == s_Presets.end()) {
			return;
		}
		LOG_DEBUG("BGB preset {} renamed to {}", oldName, newName);
		it->SetPresetName(newName);
	}

	void LoadBGBImages()
	{
		LOG_DEBUG("BGB images load start");
		std::thread([]() {
			for (const BGB& bgb : bgbs) {
				std::string file = std::format("Images\\Gum Images\\{}\\{}.png", bgb.Category, bgb.Name);
				if (!std::filesystem::exists(file)) {
					continue;
				}
				BGBImage image;
				image.BGBData = bgb;
				image.Image = std::make_shared<Walnut::Image>(file);
				bgbImages.insert({ bgb.Name, image });
			}
			}).join();
		bgbDisplayImages = BGBSearch(bgbDisplayImages, "");
		LOG_DEBUG("BGB images load end");
	}

	void LoadSingleBGBImage(const std::string& name)
	{
		const auto& bgb = std::find(bgbs.begin(), bgbs.end(), name);
		std::string file = std::format("Images\\Gum Images\\{}\\{}.png", bgb->Category, bgb->Name);
		BGBImage image;
		image.BGBData = *bgb;
		image.Image = std::make_shared<Walnut::Image>(file);
		bgbImages.insert({ bgb->Name, image });
	}

	void UnloadBGBImages()
	{
		LOG_DEBUG("BGB images unloading");
		bgbImages.clear();
		bgbDisplayImages.clear();
	}

	std::vector<std::shared_ptr<BGBImage>> BGBSearch(const std::vector<std::shared_ptr<BGBImage>>& images, std::string_view text)
	{
		std::vector<std::shared_ptr<BGBImage>> bgbsOut;
		std::string textLower = text.data();
		std::transform(textLower.begin(), textLower.end(), textLower.begin(), [](char c) { return std::tolower(c); });

		const std::vector<std::string>& bgbCompare = bgbClassicSelection ? bgbClassics : bgbMegas;

		for (const std::string& name : bgbCompare) {
			std::string lower = name;
			std::transform(lower.begin(), lower.end(), lower.begin(), [](char c) { return std::tolower(c); });
			if (lower.find(textLower) != std::string::npos) {
				bgbsOut.emplace_back(std::make_shared<BGBImage>(bgbImages[name]));
			}
		}

		std::sort(bgbsOut.begin(), bgbsOut.end(), CompareImagePointer);

		return bgbsOut;
	}
}