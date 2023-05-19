#include "Keybinds.h"
#include "ToolData.h"
#include "GlobalData.h"
#include "Walnut/Application.h"
#include "imgui_impl_glfw.h"

static int modsUsed = 0;

namespace BO3PT
{
	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (action != GLFW_PRESS && action != GLFW_RELEASE)
			return;

		ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);

		if (!registerHotKey)
			return;

		if (action == GLFW_RELEASE)
		{
			bool keyPressed = false;
			for (ImGuiKey key = (ImGuiKey)0; key < ImGuiKey_NamedKey_END; key = (ImGuiKey)(key + 1))
			{
				if (keyPressed = ImGui::IsKeyDown(key))
					break;

			}
			if (!keyPressed)
			{
				registerHotKey = false;
				modsUsed = 0;
				return;
			}
		}
		else if (ImGui::IsKeyPressed(ImGuiKey_Escape))
		{
			modsToAssign = { };
			hotkeyToAssign->second.keys = { };
			hotkeyToAssign->second.keyNames = "Unbound";
			hotkeyToAssign = nullptr;
			registerHotKey = false;
			modsUsed = 0;
			return;
		}
		else if (action == GLFW_PRESS)
		{
			if (modsUsed > 1 && mods || modsToAssign.find(key) != modsToAssign.end())
				return;
			modsUsed += (mods > 0);
			modsToAssign.insert(key);
			if (!mods)
			{
				hotkeyToAssign->second.keys.reserve(modsToAssign.size() + 1);
				std::copy(modsToAssign.begin(), modsToAssign.end(), std::back_inserter(hotkeyToAssign->second.keys));
				hotkeyToAssign->second.keys.emplace_back(key);
				for (const int& modKey : modsToAssign)
					hotkeyToAssign->second.keyNames += ImGui::GetKeyName((ImGuiKey)modKey) + std::string("+");
				hotkeyToAssign->second.keyNames += ImGui::GetKeyName((ImGuiKey)key);
				modsToAssign = { };
				hotkeyToAssign = nullptr;
				registerHotKey = false;
				modsUsed = 0;
			}
		}
	}

	void GodmodeOn()
	{
		godActive = !godActive;
		NotifyGame({ 0, 11, godActive });
	}
	void InfiniteAmmo()
	{
		infAmmoActive = !infAmmoActive;
		NotifyGame({ 0, 12, infAmmoActive });
	}
	void TimescaleIncrease()
	{
		if (timescaleInt < 10)
			timescaleInt++;
		NotifyGame({ 0, 13, timescaleInt });
	}
	void TimescaleDecrease()
	{
		if (timescaleInt > 1)
			timescaleInt--;
		NotifyGame({ 0, 13, timescaleInt });
	}
	void MaxPoints()
	{
		NotifyGame({ 0, 0, 4194303 });
	}
	void ResetPoints()
	{
		NotifyGame({ 0, 0, 500 });
	}
	void GiveAllPerks()
	{
		NotifyGame({ 0, 7 });
	}
	void TakeAllPerks()
	{
		NotifyGame({ 0, 8 });
	}
	void TakeGum()
	{
		NotifyGame({ 0, 4 });
	}
	void TakeGumCharge()
	{
		NotifyGame({ 0, 5 });
	}
	void ActivateGum()
	{
		NotifyGame({ 0, 6 });
	}
	void ZombiesIgnorePlayer()
	{
		NotifyGame({ 1, 0, zombiesIgnore });
	}
	void KillHorde()
	{
		NotifyGame({ 1, 1 });
	}
	void FreezeZombies()
	{
		NotifyGame({ 1, 2, zombiesFreeze });
	}
	void ToggleSpawning()
	{
		NotifyGame({ 1, 3, zombiesSpawn });
	}
	void ZombiesWalk()
	{
		zombieSpeedRun = false;
		zombieSpeedSprint = false;
		if (zombieSpeedWalk)
			NotifyGame({ 1, 4, 0 });
		else
			NotifyGame({ 1, 4, 3 });
	}
	void ZombiesRun()
	{
		zombieSpeedWalk = false;
		zombieSpeedSprint = false;
		if (zombieSpeedRun)
			NotifyGame({ 1, 4, 1 });
		else
			NotifyGame({ 1, 4, 3 });
	}
	void ZombiesSprint()
	{
		zombieSpeedWalk = false;
		zombieSpeedRun = false;
		if (zombieSpeedSprint)
			NotifyGame({ 1, 4, 2 });
		else
			NotifyGame({ 1, 4, 3 });
	}
	void EndRound()
	{
		NotifyGame({ 2, 0 });
	}
	void RestartRound()
	{
		NotifyGame({ 2, 1 });
	}
	void PickupEveryPart()
	{
		NotifyGame({ 5, 0 });
	}
	void OpenAllDoors()
	{
		NotifyGame({ 6, 0 });
	}
	void GlobalPowerOn()
	{
		NotifyGame({ 6, 2 });
	}
	void OpenAllBarriers()
	{
		NotifyGame({ 6, 5, 0 });
	}
	void CloseAllBarriers()
	{
		NotifyGame({ 6, 5, 1 });
	}
}