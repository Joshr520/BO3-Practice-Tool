#pragma once

#include "ImGui/ImGuiExtendedWidgets.h"

#include <string>
#include <vector>
#include <unordered_map>

namespace BO3PracticeTool
{
	struct Split {
		std::string Name;
		std::string CustomName;
		int Round;

		bool operator==(const Split& split) { return Name == split.Name; }
		bool operator==(const std::string& name) { return Name == name; }
	};

	struct AutosplitData {
		std::vector<Split> Splits = { };
		bool IGT = false;
		bool IGRT = false;
		size_t NumSplits = 0;
		size_t MapNum = 0;
		size_t SplitType = 0;
	};

	class AutosplitPreset
	{
	public:
		AutosplitPreset(std::string_view name) : m_Name(name) { }

		size_t& GetCondition(size_t index) { return m_Conditions[index]; }
		std::string_view GetPresetName() const { return m_Name; }
		AutosplitData& GetData() { return m_Data; }
		static AutosplitPreset* GetCurrentPreset() { return s_CurrentPreset; }

		void AddSplit(Split split);
		void DeleteSplit(size_t index);
		void MarkSwap(size_t source, size_t target) { m_SplitDragIndexes = { source, target }; m_ShouldMove = true; };
		bool ShouldMove() const { return m_ShouldMove; }
		void MoveSplit();
		void SetPresetName(std::string_view name) { m_Name = name; }

		void SavePreset();
		static void SavePreset(std::string_view name);
		static void WritePresetToGame(bool reset = false);

		static void LoadAutosplitPresets();
		static void AddAutosplitPreset(std::string_view name);
		static void DeleteAutosplitPreset(std::string_view name);
		static void AutosplitPresetIndexChanged(size_t index) { s_CurrentPreset = &s_Presets[index]; }
		static void EditedAutosplitPreset(std::string_view oldName, std::string_view newName);

		bool operator==(const AutosplitPreset& preset) const { return m_Name == preset.m_Name; }
	private:
		std::vector<size_t> m_Conditions = { };
		std::string m_Name;
		AutosplitData m_Data;
		bool m_ShouldMove = false;
		std::pair<size_t, size_t> m_SplitDragIndexes = { };

		static std::vector<AutosplitPreset> s_Presets;
		static AutosplitPreset* s_CurrentPreset;
	};

	inline bool autosplitPresetsActive = false;
	inline ImGui::FileEditor autosplitFiles(ImGui::FileEditor{ "Autosplit Preset", std::vector<ImGui::TextEditSelectable>{  } });
}