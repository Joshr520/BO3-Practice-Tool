#include "BGBData.h"

#include "PracticeTool/Core/Log.h"
#include "PracticeTool/LibHelpers/json.h"
#include "PracticeTool/LibHelpers/misc.h"
#include "PracticeTool/UI/Notifications.h"

#include <filesystem>

#define BGB_DIRECTORY "Settings\\BGB Presets\\"
#define BGB_FILENAME(name) std::format(BGB_DIRECTORY "{}.json", name)

namespace BO3PracticeTool
{
	void BGBPreset::SavePreset()
	{
		JSON json;

		rapidjson::Value& gums = json.AddArray(json.GetDocument(), "Gums");
		for (const BGB* bgb : m_BGBs) {
			rapidjson::Value gumValue;
			gumValue.SetString(rapidjson::StringRef(bgb->Name.c_str()));
			gums.PushBack(gumValue, json.GetAllocator());
		}

		json.SaveToFile(BGB_FILENAME(m_Name));
	}

	void BGBPreset::SavePreset(std::string_view name)
	{
		auto it = std::find(bgbPresets.begin(), bgbPresets.end(), name);
		if (it != bgbPresets.end()) {
			it->SavePreset();
		}
	}

	void LoadBGBPresets()
	{
		LOG_DEBUG("Loading BGB Presets");

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

			if (!JSON::IsArray(json.GetDocument(), "Gums")) {
				continue;
			}

			const rapidjson::Value& gumsArray = document["Gums"];
			bool valid = true;

			for (rapidjson::SizeType i = 0; i < gumsArray.Size(); i++) {
				if (gumsArray[i].IsString()) {
					auto it = bgbNameToIndex.find(gumsArray[i].GetString());
					if (it != bgbNameToIndex.end()) {
						BGB* bgb = &bgbs[it->second];
						preset.SetBGB(i, bgb);
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
			bgbPresets.emplace_back(preset);
			bgbFiles.AddFile(response.NewFile);
		}

		bgbFiles.ReformatFiles();
		bgbFiles.SetCallbackNew(AddBGBPreset);
		bgbFiles.SetCallbackDelete(DeleteBGBPreset);
		bgbFiles.SetCallbackLastSelectedChanged(BGBPresetIndexChanged);
		bgbFiles.SetCallbackEdited(EditedBGBPreset);
	}

	void AddBGBPreset(std::string_view name)
	{
		bgbPresets.emplace_back(BGBPreset(name));
		if (!CheckDirectory(BGB_DIRECTORY)) {
			return;
		}
		BGBPreset::SavePreset(name);
	}

	void DeleteBGBPreset(std::string_view name)
	{
		if (bgbPresetsIndex && bgbPresetsIndex == bgbPresets.size() - 1) {
			bgbPresetsIndex--;
		}
		SafeEraseBGBPreset(bgbPresets, name);
	}

	void EditedBGBPreset(std::string_view oldName, std::string_view newName)
	{
		auto it = std::find(bgbPresets.begin(), bgbPresets.end(), oldName);
		if (it == bgbPresets.end()) {
			return;
		}
		it->SetPresetName(newName);
	}

	void LoadBGBImages()
	{
		LOG_DEBUG("BGB Image Load Start");
		std::thread([]() {
			for (const BGB& bgb : bgbs) {
				std::string file = std::format("Images\\Gum Images\\{}\\{}.png", bgb.Category, bgb.Name);
				if (!std::filesystem::exists(file)) {
					continue;
				}
				BGBImage image;
				image.BGBData = bgb;
				image.Image = std::make_shared<Walnut::Image>(file);
				bgb.Category == "Classic" ? bgbClassicImages.emplace_back(std::move(image)) : bgbMegaImages.emplace_back(std::move(image));
			}
			std::sort(bgbClassicImages.begin(), bgbClassicImages.end());
			std::sort(bgbMegaImages.begin(), bgbMegaImages.end());

			for (const BGBImage& bgb : bgbClassicImages) {
				BO3PracticeTool::bgbCombinedImages.emplace_back(std::make_shared<BGBImage>(bgb));
				BO3PracticeTool::bgbDisplayImages.emplace_back(std::make_shared<BGBImage>(bgb));
			}
			for (const BGBImage& bgb : bgbMegaImages) {
				bgbCombinedImages.emplace_back(std::make_shared<BGBImage>(bgb));
			}
			}).join();
		LOG_DEBUG("BGB Image Load End");
	}

	std::vector<std::shared_ptr<BGBImage>> BGBSearch(const std::vector<std::shared_ptr<BGBImage>>& images, std::string_view text)
	{
		std::vector<std::shared_ptr<BGBImage>> bgbsOut;
		std::string textLower = text.data();
		std::transform(textLower.begin(), textLower.end(), textLower.begin(), [](char c) { return std::tolower(c); });

		const std::vector<BGBImage>& bgbCompare = bgbClassicSelection ? bgbClassicImages : bgbMegaImages;

		for (const BGBImage& bgb : bgbCompare) {
			std::string name = bgb.BGBData.Name;
			std::transform(name.begin(), name.end(), name.begin(), [](char c) { return std::tolower(c); });
			if (name.find(textLower) != std::string::npos) {
				bgbsOut.emplace_back(std::make_shared<BGBImage>(bgb));
			}
		}

		std::sort(bgbsOut.begin(), bgbsOut.end(), CompareImagePointer);

		return bgbsOut;
	}
}