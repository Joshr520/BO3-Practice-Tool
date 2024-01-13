#include "AutosplitData.h"

#include "PracticeTool/Core/Log.h"
#include "PracticeTool/LibHelpers/json.h"
#include "PracticeTool/LibHelpers/misc.h"
#include "PracticeTool/Core/Memory.h"
#include "PracticeTool/StructDefs/GameData.h"
#include "PracticeTool/UI/WindowBody.h"

#include <fstream>

#define AUTOSPLIT_DIRECTORY "Settings\\Autosplit Presets\\"
#define AUTOSPLIT_FILENAME(name) std::format(AUTOSPLIT_DIRECTORY "{}.json", name)

namespace BO3PracticeTool
{
	std::vector<AutosplitPreset> AutosplitPreset::s_Presets = { };
	AutosplitPreset* AutosplitPreset::s_CurrentPreset = nullptr;

	void SafeEraseAutosplitPreset(std::vector<AutosplitPreset>& presets, std::string_view presetName)
	{
		presets.erase(std::remove_if(presets.begin(), presets.end(), [&](const AutosplitPreset& preset) {
			return preset.GetPresetName() == presetName;
			}), presets.end());
	}

	void AutosplitPreset::SavePreset()
	{
		JSON json;
		std::ofstream file;
		bool writeToGame = Memory::injected && autosplitPresetsActive;

		if (writeToGame) {
			file.open(practiceToolDirectory + "\\Autosplit Preset.txt");
		}

		rapidjson::Value& settings = json.AddObject(json.GetDocument(), "Settings");
		settings.AddMember("In Game Timer", m_Data.IGT, json.GetAllocator());
		settings.AddMember("In Game Round Timer", m_Data.IGRT, json.GetAllocator());
		settings.AddMember("Amount of Splits", m_Data.NumSplits, json.GetAllocator());
		settings.AddMember("Map Index", m_Data.MapNum, json.GetAllocator());
		settings.AddMember("Split Type", m_Data.SplitType, json.GetAllocator());

		if (writeToGame) {
			file << t7MapsInternal[m_Data.MapNum] << ",";
			file << m_Data.IGT << "," << m_Data.IGRT << ",";
			file << m_Data.NumSplits << "," << m_Data.SplitType << "\n";
		}

		rapidjson::Value& data = json.AddObject(json.GetDocument(), "Split Data");
		for (const Split& split : m_Data.Splits) {
			rapidjson::Value& splitObject = json.AddObject(data, split.Name);
			splitObject.AddMember("Custom Name", rapidjson::StringRef(split.CustomName.c_str()), json.GetAllocator());
			splitObject.AddMember("Round", split.Round, json.GetAllocator());
			if (writeToGame) {
				file << split.Name << "," << split.CustomName << "," << split.Round << "\n";
			}
		}

		if (file.is_open()) {
			file.close();
		}

		json.SaveToFile(AUTOSPLIT_FILENAME(m_Name));
	}

	void AutosplitPreset::SavePreset(std::string_view name)
	{
		auto it = std::find(s_Presets.begin(), s_Presets.end(), name);
		if (it != s_Presets.end()) {
			it->SavePreset();
		}
	}

	void AutosplitPreset::WritePresetToGame(bool reset)
	{
		AutosplitPreset* currentPreset = AutosplitPreset::GetCurrentPreset();

		if (!currentPreset || reset) {
			std::ofstream(practiceToolDirectory + "\\Autosplit Preset.txt");
			return;
		}

		std::ofstream file(practiceToolDirectory + "\\Autosplit Preset.txt");
		AutosplitData currentData = currentPreset->GetData();

		file << t7MapsInternal[currentData.MapNum] << ",";
		file << currentData.IGT << "," << currentData.IGRT << ",";
		file << currentData.NumSplits << "," << currentData.SplitType << "\n";

		for (const Split& split : currentData.Splits) {
			file << split.Name << "," << split.CustomName << "," << split.Round << "\n";
		}

		file.close();
	}

	void AutosplitPreset::LoadAutosplitPresets()
	{
		LOG_DEBUG("Loading autosplit presets");

		if (!CheckDirectory(AUTOSPLIT_DIRECTORY)) {
			return;
		}

		autosplitFiles.SetFileExt(".json");
		autosplitFiles.SetDirectory(AUTOSPLIT_DIRECTORY);

		for (const auto& directory : std::filesystem::directory_iterator(AUTOSPLIT_DIRECTORY)) {
			const std::string filename = directory.path().filename().stem().string();
			AutosplitPreset preset(filename);
			JSON json = JSON::FromFile(directory.path().string());
			const rapidjson::Document& document = json.GetDocument();

			if (json.HasMember(document, "Settings")) {
				const rapidjson::Value& settings = document["Settings"];

				JSONGetResult igt = json.GetBool(settings, "In Game Timer");
				JSONGetResult igrt = json.GetBool(settings, "In Game Round Timer");
				JSONGetResult numSplits = json.GetInt(settings, "Amount of Splits");
				JSONGetResult mapNum = json.GetInt(settings, "Map Index");
				JSONGetResult splitType = json.GetInt(settings, "Split Type");

				igt.GetSuccess ? preset.m_Data.IGT = igt.ReturnValue : false;
				igrt.GetSuccess ? preset.m_Data.IGRT = igrt.ReturnValue : false;
				numSplits.GetSuccess ? preset.m_Data.NumSplits = numSplits.ReturnValue : 0;
				mapNum.GetSuccess ? preset.m_Data.MapNum = mapNum.ReturnValue : 0;
				splitType.GetSuccess ? preset.m_Data.SplitType = splitType.ReturnValue : 0;
			}
			if (json.IsObject(document, "Split Data")) {
				const rapidjson::Value& splitData = document["Split Data"];

				for (rapidjson::Value::ConstMemberIterator it = splitData.MemberBegin(); it != splitData.MemberEnd(); it++) {
					JSONGetResult CustomName = json.GetString(it->value, "Custom Name");
					if (!CustomName.GetSuccess) {
						continue;
					}
					JSONGetResult Round = json.GetInt(it->value, "Round");
					if (!Round.GetSuccess) {
						continue;
					}

					Split split = Split(it->name.GetString(), CustomName.ReturnValue, Round.ReturnValue);
					preset.m_Data.Splits.emplace_back(split);
					preset.m_Conditions.emplace_back(split.Round <= 0 ? (!split.Round ? 1 : 0) : 2);
				}
			}

			ImGui::CreateFileResponse response = autosplitFiles.CreateEmptyFile(filename);
			if (!response.Created) {
				continue;
			}
			s_Presets.emplace_back(preset);
			autosplitFiles.AddFile(response.NewFile);
		}

		if (s_Presets.size()) {
			s_CurrentPreset = &s_Presets[0];
		}

		autosplitFiles.ReformatFiles();
		autosplitFiles.SetCallbackNew(AddAutosplitPreset);
		autosplitFiles.SetCallbackDelete(DeleteAutosplitPreset);
		autosplitFiles.SetCallbackLastSelectedChanged(AutosplitPresetIndexChanged);
		autosplitFiles.SetCallbackEdited(EditedAutosplitPreset);
	}

	void AutosplitPreset::AddSplit(Split split)
	{
		m_Data.Splits.emplace_back(split);
		m_Data.NumSplits++;
		int condition = split.Round <= 0 ? (!split.Round ? 1 : 0) : 2;
		m_Conditions.emplace_back(condition);
		s_CurrentPreset->SavePreset();
	}

	void AutosplitPreset::DeleteSplit(size_t index)
	{
		m_Data.Splits.erase(m_Data.Splits.begin() + index);
		m_Conditions.erase(m_Conditions.begin() + index);
		m_Data.NumSplits--;
		s_CurrentPreset->SavePreset();
	}

	void AutosplitPreset::MoveSplit()
	{
		size_t source = m_SplitDragIndexes.first;
		size_t target = m_SplitDragIndexes.second;
		if (source != target && source < m_Data.Splits.size() && target < m_Data.Splits.size()) {
			if (source < target) {
				for (size_t i = source; i < target; ++i) {
					std::swap(m_Data.Splits[i], m_Data.Splits[i + 1]);
					std::swap(m_Conditions[i], m_Conditions[i + 1]);
				}
			}
			else {
				for (size_t i = source; i > target; --i) {
					std::swap(m_Data.Splits[i], m_Data.Splits[i - 1]);
					std::swap(m_Conditions[i], m_Conditions[i - 1]);
				}
			}

			m_ShouldMove = false;
			s_CurrentPreset->SavePreset();
		}
	}

	void AutosplitPreset::AddAutosplitPreset(std::string_view name)
	{
		if (!CheckDirectory(AUTOSPLIT_DIRECTORY)) {
			return;
		}
		LOG_DEBUG("Autosplit preset {} created", name);
		AutosplitPreset preset(name);
		s_Presets.emplace_back(preset);
		s_CurrentPreset = &s_Presets[s_Presets.size() - 1];
		AutosplitPreset::SavePreset(name);
	}

	void AutosplitPreset::DeleteAutosplitPreset(std::string_view name)
	{
		if (s_CurrentPreset->m_Name == s_Presets[s_Presets.size() - 1].m_Name) {
			if (s_Presets.size() > 1) {
				s_CurrentPreset = &s_Presets[s_Presets.size() - 2];
			}
			else {
				s_CurrentPreset = nullptr;
			}
		}
		LOG_DEBUG("Weapon preset {} deleted", name);
		SafeEraseAutosplitPreset(s_Presets, name);
	}

	void AutosplitPreset::EditedAutosplitPreset(std::string_view oldName, std::string_view newName)
	{
		auto it = std::find(s_Presets.begin(), s_Presets.end(), oldName);
		if (it == s_Presets.end()) {
			return;
		}
		LOG_DEBUG("Weapon preset {} renamed to {}", oldName, newName);
		it->SetPresetName(newName);
	}
}