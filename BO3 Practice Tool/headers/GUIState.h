#pragma once

#include <string>
#include <vector>
#include <functional>

typedef int ImGuiWindowFlags;

namespace BO3PT {

	typedef int GUIStates;

	enum GUIStates_ {
		SteamFound = 1 << 0,
		SetupDone = 1 << 1,
		Docked = 1 << 2,
		Active = 1 << 3
	};

	struct GUIMember {
		std::function<void()> renderFunc = nullptr;
	};

	class GUIState {
	public:
		static void Render() { s_Members[s_Index].renderFunc(); }

		static GUIStates GetState() { return s_State; }
		static int GetIndex() { return s_Index; }
		static bool IsStateSet(GUIStates state) { return (s_State & state) == state; }

		static void SetState(GUIStates state) { s_State |= state; }
		static void UnsetState(GUIStates state) { s_State &= ~state; }
		static void SetIndex(int index) { s_Index = index; }

		static void AddMember(const GUIMember& member) { s_Members.emplace_back(member); };
	private:
		static std::vector<GUIMember> s_Members;
		static GUIStates s_State;
		static int s_Index;
	};

}