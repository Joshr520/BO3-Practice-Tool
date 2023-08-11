#include "WeaponData.h"

#include "PracticeTool/Core/Log.h"
#include "PracticeTool/LibHelpers/json.h"
#include "PracticeTool/LibHelpers/misc.h"
#include "PracticeTool/UI/Notifications.h"

#include <filesystem>

#define WEAPON_DIRECTORY "Settings\\Weapon Presets\\"
#define WEAPON_FILENAME(name) std::format(WEAPON_DIRECTORY "{}.json", name)

namespace BO3PracticeTool
{
	void WeaponPreset::SavePreset()
	{
		JSON json;

		for (size_t i = 0; i < weaponList.Types.size(); i++) {
			const std::string& type = weaponList.Types[i];
			rapidjson::Value& typeObject = json.AddObject(json.GetDocument(), type);

			for (size_t j = 0; j < weaponList.Weapons.size(); j++) {
				const Weapon& weapon = weaponList.Weapons[i][j];
				const WeaponItem weaponItem = m_Items.find(type)->second[j];

				rapidjson::Value& weaponObject = json.AddObject(typeObject, weapon.Name);
				weaponObject.AddMember("Optic", weaponItem.Optic, json.GetAllocator());

				rapidjson::Value& attachments = json.AddArray(weaponObject, "Attachments");
				for (const size_t& attachment : weaponItem.Attachments) {
					attachments.PushBack(attachment, json.GetAllocator());
				}
			}
		}

		json.SaveToFile(WEAPON_FILENAME(m_Name));
	}

	void WeaponPreset::SavePreset(std::string_view name)
	{
		auto it = std::find(weaponPresets.begin(), weaponPresets.end(), name);
		if (it != weaponPresets.end()) {
			it->SavePreset();
		}
	}

	void LoadWeaponPresets()
	{
		LOG_DEBUG("Loading Weapon");

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

			for (const std::string& type : weaponList.Types) {
				std::vector<WeaponItem> typeEntry;
				if (json.IsObject(document, type)) {
					const rapidjson::Value& typeObject = document[type.c_str()];

					for (rapidjson::Value::ConstMemberIterator it = typeObject.MemberBegin(); it != typeObject.MemberEnd(); it++) {
						const std::string& name = it->name.GetString();
						const rapidjson::Value& value = it->value;
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
				preset.AddWeapons(type, typeEntry );
			}

			weaponPresets.emplace_back(preset);

			ImGui::CreateFileResponse response = weaponFiles.CreateEmptyFile(filename);
			if (!response.Created) {
				continue;
			}
			weaponFiles.AddFile(response.NewFile);
		}

		weaponFiles.ReformatFiles();
		weaponFiles.SetCallbackNew(AddWeaponPreset);
		weaponFiles.SetCallbackDelete(DeleteWeaponPreset);
		weaponFiles.SetCallbackLastSelectedChanged(WeaponPresetIndexChanged);
		weaponFiles.SetCallbackEdited(EditedWeaponPreset);
	}

	void AddWeaponPreset(std::string_view name)
	{
		weaponPresets.emplace_back(WeaponPreset(name));
		if (!CheckDirectory(WEAPON_DIRECTORY)) {
			return;
		}
		WeaponPreset::SavePreset(name);
	}

	void DeleteWeaponPreset(std::string_view name)
	{
		if (weaponPresetsIndex == weaponPresets.size() - 1) {
			weaponPresetsIndex--;
		}
		SafeEraseWeaponPreset(weaponPresets, name);
	}

	void EditedWeaponPreset(std::string_view oldName, std::string_view newName)
	{
		auto it = std::find(weaponPresets.begin(), weaponPresets.end(), oldName);
		if (it == weaponPresets.end()) {
			return;
		}
		it->SetPresetName(newName);
	}

	void LoadWeaponImages()
	{
		LOG_DEBUG("Weapon Image Load Start");
		std::thread buildKitThread([&]() {
			for (const auto& entry : std::filesystem::directory_iterator("Images\\Weapons\\Build Kits")) {
				if (entry.path().extension().string() != ".png") {
					continue;
				}
				buildKitImgList.emplace_back(std::make_unique<Walnut::Image>(entry.path().string()));
			}
			});

		std::thread camosThread([&]() {
			for (const auto& directory : std::filesystem::directory_iterator("Images\\Weapons\\Camos")) {
				if (std::filesystem::is_directory(directory.path())) {
					CamoGroup camoGroup;
					for (const auto& file : std::filesystem::directory_iterator(directory.path())) {
						if (file.path().extension().string() != ".png") {
							continue;
						}
						camoGroup.Camos.insert({ file.path().filename().stem().string(), std::make_unique<Walnut::Image>(file.path().string()) });
					}
					camosImgList.insert({ directory.path().filename().stem().string(), std::move(camoGroup) });
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

		buildKitThread.join();
		camosThread.join();
		weaponIconsThread.join();
		opticsThread.join();
		attachmentsThread.join();
		LOG_DEBUG("Weapon Image Load End");
	}
}