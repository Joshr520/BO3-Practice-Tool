#include "WeaponData.h"

#include "PracticeTool/Core/Log.h"
#include "PracticeTool/LibHelpers/json.h"
#include "PracticeTool/LibHelpers/misc.h"
#include "PracticeTool/Core/Memory.h"
#include "PracticeTool/StructDefs/GameData.h"
#include "PracticeTool/UI/WindowBody.h"

#include <filesystem>
#include <fstream>

#define WEAPON_DIRECTORY "Settings\\Weapon Presets\\"
#define WEAPON_FILENAME(name) std::format(WEAPON_DIRECTORY "{}.json", name)

namespace BO3PracticeTool
{
	std::vector<WeaponPreset> WeaponPreset::s_Presets = { };
	WeaponPreset* WeaponPreset::s_CurrentPreset = nullptr;

	void SafeEraseWeaponPreset(std::vector<WeaponPreset>& presets, std::string_view presetName)
	{
		presets.erase(std::remove_if(presets.begin(), presets.end(), [&](const WeaponPreset& preset) {
			return preset.GetPresetName() == presetName;
			}), presets.end());
	}

	void WeaponPreset::SavePreset()
	{
		JSON json;
		std::ofstream file;
		bool writeToGame = Memory::injected && weaponPresetsActive;

		if (writeToGame) {
			file.open(practiceToolDirectory + "\\Weapon Preset.txt");
		}

		for (size_t i = 0; i < 6; i++) {
			const std::string& type = weaponTypes[i];
			rapidjson::Value& typeObject = json.AddObject(json.GetDocument(), type);

			for (size_t j = 0; j < menuWeapons[i].size(); j++) {
				const Weapon& weapon = menuWeapons[i][j];
				const WeaponItem& item = m_Items[type.data()][j];
				if (writeToGame) {
					file << weapon.InternalName << ",";
					if (item.Optic >= 0) {
						file << weapon.Optics[item.Optic].InternalName << ",";
					}
					for (const size_t& attachment : item.Attachments) {
						file << weapon.Attachments[attachment].InternalName << ",";
					}
					file << item.Camo << "\n";
				}

				rapidjson::Value& weaponObject = json.AddObject(typeObject, weapon.InternalName);
				weaponObject.AddMember("Optic", item.Optic, json.GetAllocator());

				rapidjson::Value& attachments = json.AddArray(weaponObject, "Attachments");
				for (const size_t& attachment : item.Attachments) {
					attachments.PushBack(attachment, json.GetAllocator());
				}

				weaponObject.AddMember("Camo", item.Camo, json.GetAllocator());
			}
		}

		if (file.is_open()) {
			file.close();
		}

		json.SaveToFile(WEAPON_FILENAME(m_Name));
	}

	void WeaponPreset::SavePreset(std::string_view name)
	{
		auto it = std::find(s_Presets.begin(), s_Presets.end(), name);
		if (it != s_Presets.end()) {
			it->SavePreset();
		}
	}

	void WeaponPreset::WritePresetToGame(bool reset)
	{
		WeaponPreset* currentPreset = WeaponPreset::GetCurrentPreset();

		if (!currentPreset || reset) {
			std::ofstream(practiceToolDirectory + "\\Weapon Preset.txt");
			return;
		}

		std::ofstream file(practiceToolDirectory + "\\Weapon Preset.txt");

		for (size_t i = 0; i < 6; i++) {
			const std::string& type = weaponTypes[i];

			for (size_t j = 0; j < menuWeapons[i].size(); j++) {
				const Weapon& weapon = menuWeapons[i][j];
				const WeaponItem& item = currentPreset->GetWeaponItem(type, j);
				file << weapon.InternalName << ",";
				if (item.Optic >= 0) {
					file << weapon.Optics[item.Optic].InternalName << ",";
				}
				for (const size_t& attachment : item.Attachments) {
					file << weapon.Attachments[attachment].InternalName << ",";
				}
				file << item.Camo << "\n";
			}
		}

		file.close();
	}

	void WeaponPreset::LoadWeaponPresets()
	{
		LOG_DEBUG("Loading weapon presets");

		if (!CheckDirectory(WEAPON_DIRECTORY)) {
			return;
		}

		weaponFiles.SetFileExt(".json");
		weaponFiles.SetDirectory(WEAPON_DIRECTORY);

		for (const auto& directory : std::filesystem::directory_iterator(WEAPON_DIRECTORY)) {
			const std::string filename = directory.path().filename().stem().string();
			WeaponPreset preset(filename);
			JSON json = JSON::FromFile(directory.path().string());
			const rapidjson::Document& document = json.GetDocument();

			for (size_t i = 0; i < weaponTypes.size(); i++) {
				const std::string& type = weaponTypes[i];

				std::vector<WeaponItem> typeEntry;
				if (json.IsObject(document, type)) {
					const rapidjson::Value& typeObject = document[type.c_str()];

					for (rapidjson::Value::ConstMemberIterator it = typeObject.MemberBegin(); it != typeObject.MemberEnd(); it++) {
						const std::string& name = it->name.GetString();
						const rapidjson::Value& value = it->value;
						const Weapon& weapon = menuWeapons[i][std::distance(typeObject.MemberBegin(), it)];
						WeaponItem weaponEntry;

						JSONGetResult optic = json.GetInt(value, "Optic");
						if (optic.GetSuccess) {
							weaponEntry.Optic = optic.ReturnValue;
						}

						if (json.IsArray(value, "Attachments")) {
							const rapidjson::Value& attachments = value["Attachments"];

							for (rapidjson::SizeType i = 0; i < attachments.Size(); i++) {
								if (attachments[i].IsInt()) {
									weaponEntry.Attachments.emplace_back(attachments[i].GetInt());
								}
							}
						}

						JSONGetResult camo = json.GetInt(value, "Camo");
						if (camo.GetSuccess) {
							weaponEntry.Camo = camo.ReturnValue;
						}

						typeEntry.emplace_back(weaponEntry);
					}
				}
				preset.AddWeapons(type, typeEntry);
			}

			s_Presets.emplace_back(preset);

			ImGui::CreateFileResponse response = weaponFiles.CreateEmptyFile(filename);
			if (!response.Created) {
				continue;
			}
			weaponFiles.AddFile(response.NewFile);
		}

		if (s_Presets.size()) {
			s_CurrentPreset = &s_Presets[0];
			const std::string& type = weaponTypes[weaponPresetsMenu];
			WeaponItem& item = s_CurrentPreset->GetWeaponItem(type, weaponBeingEdited);
			if (item.Optic >= 0) {
				weaponOpticSelection.SetSelected(item.Optic);
			}
			for (size_t attachment : item.Attachments) {
				weaponAttachmentsSelection.SetSelected(attachment);
			}
		}

		weaponFiles.ReformatFiles();
		weaponFiles.SetCallbackNew(AddWeaponPreset);
		weaponFiles.SetCallbackDelete(DeleteWeaponPreset);
		weaponFiles.SetCallbackLastSelectedChanged(WeaponPresetIndexChanged);
		weaponFiles.SetCallbackEdited(EditedWeaponPreset);
	}

	void WeaponPreset::AddWeaponPreset(std::string_view name)
	{
		if (!CheckDirectory(WEAPON_DIRECTORY)) {
			return;
		}
		LOG_DEBUG("Weapon preset {} created", name);
		WeaponPreset preset(name);
		for (size_t i = 0; i < weaponTypes.size(); i++) {
			const std::string& type = weaponTypes[i];
			std::vector<WeaponItem> weaponEntry;
			for (int i = 0; i < menuWeapons.size(); i++) {
				weaponEntry.emplace_back(WeaponItem(-1, { }, -1));
			}
			preset.AddWeapons(type, weaponEntry);
		}
		s_Presets.emplace_back(preset);
		s_CurrentPreset = &s_Presets[s_Presets.size() - 1];
		weaponOpticSelection.Deselect();
		weaponAttachmentsSelection.ClearAll();
		WeaponPreset::SavePreset(name);
	}

	void WeaponPreset::DeleteWeaponPreset(std::string_view name)
	{
		weaponOpticSelection.Deselect();
		weaponAttachmentsSelection.ClearAll();
		if (s_CurrentPreset->m_Name == s_Presets[s_Presets.size() - 1].m_Name) {
			if (s_Presets.size() > 1) {
				s_CurrentPreset = &s_Presets[s_Presets.size() - 2];
				const std::string& type = weaponTypes[weaponPresetsMenu];
				WeaponItem& item = s_CurrentPreset->GetWeaponItem(type, weaponBeingEdited);
				if (item.Optic >= 0) {
					weaponOpticSelection.SetSelected(item.Optic);
				}
				for (size_t attachment : item.Attachments) {
					weaponAttachmentsSelection.SetSelected(attachment);
				}
			}
			else {
				s_CurrentPreset = nullptr;
			}
		}
		LOG_DEBUG("Weapon preset {} deleted", name);
		SafeEraseWeaponPreset(s_Presets, name);
	}

	void WeaponPreset::EditedWeaponPreset(std::string_view oldName, std::string_view newName)
	{
		auto it = std::find(s_Presets.begin(), s_Presets.end(), oldName);
		if (it == s_Presets.end()) {
			return;
		}
		LOG_DEBUG("Weapon preset {} renamed to {}", oldName, newName);
		it->SetPresetName(newName);
	}

	void WeaponPreset::ResetWeaponSelections()
	{
		weaponOpticSelection.Deselect();
		weaponAttachmentsSelection.ClearAll();
		if (!s_CurrentPreset) {
			return;
		}
		const std::string& type = weaponTypes[weaponPresetsMenu];
		WeaponItem& item = s_CurrentPreset->GetWeaponItem(type, weaponBeingEdited);
		if (item.Optic >= 0) {
			weaponOpticSelection.SetSelected(item.Optic);
		}
		for (size_t attachment : item.Attachments) {
			weaponAttachmentsSelection.SetSelected(attachment);
		}
	}

	void LoadWeaponImages()
	{
		LOG_DEBUG("Weapon images load start");

		std::thread camosThread([&]() {
			for (const auto& directory : std::filesystem::directory_iterator("Images\\Weapons\\Camos")) {
				if (std::filesystem::is_directory(directory.path())) {
					for (const auto& file : std::filesystem::directory_iterator(directory.path())) {
						if (file.path().extension().string() != ".png") {
							continue;
						}
						camosImgList.insert({ file.path().filename().stem().string(), std::make_unique<Walnut::Image>(file.path().string())});
					}
				}
			}
			});

		std::thread weaponIconsThread([&]() {
			for (const auto& directory : std::filesystem::directory_iterator("Images\\Weapons\\Icons")) {
				if (std::filesystem::is_directory(directory.path())) {
					for (const auto& file : std::filesystem::directory_iterator(directory.path())) {
						if (file.path().extension().string() != ".png") {
							continue;
						}
						weaponIconsImgList.insert({ file.path().stem().string(), std::make_unique<Walnut::Image>(file.path().string()) });
					}
				}
			}
			});

		std::thread opticsThread([&]() {
			for (const auto& entry : std::filesystem::directory_iterator("Images\\Weapons\\Attachments\\Optics")) {
				if (entry.path().extension().string() != ".png") {
					continue;
				}
				opticsImgList.insert({ entry.path().stem().string(), std::make_unique<Walnut::Image>(entry.path().string()) });
			}
			});

		std::thread attachmentsThread([&]() {
			for (const auto& directory : std::filesystem::directory_iterator("Images\\Weapons\\Attachments")) {
				if (std::filesystem::is_directory(directory.path())) {
					for (const auto& file : std::filesystem::directory_iterator(directory.path())) {
						if (file.path().extension().string() != ".png") {
							continue;
						}
						attachmentsImgList.insert({ file.path().stem().string(), std::make_unique<Walnut::Image>(file.path().string()) });
					}
				}
			}
			});

		camosThread.join();
		weaponIconsThread.join();
		opticsThread.join();
		attachmentsThread.join();
		LOG_DEBUG("Weapon images load end");
	}

	void UnloadWeaponImages()
	{
		LOG_DEBUG("Weapon images unloading");
		weaponIconsImgList.clear();
		opticsImgList.clear();
		attachmentsImgList.clear();
		camosImgList.clear();
	}

	void LoadWeaponSelections()
	{
		for (const std::string& map : t7MapsInternal) {
			ImGui::MultiSelection selection;
			for (const std::vector<Weapon>& weapons : mapWeapons[map]) {
				ImGui::SelectionData data;
				for (const Weapon& weapon : weapons) {
					data.Selection.push_back(weapon.Name);
				}
				selection.AddSelection(data);
			}
			mapWeaponsSelection.insert({ map, selection });
		}
	}
}