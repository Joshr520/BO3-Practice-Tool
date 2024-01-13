#include "KeyboardIcons.h"
#include "../res/Embed/KeyboardIcons.embed"
#include "imgui.h"

namespace BO3PracticeTool
{
	void LoadKeyboardIcons()
	{
		uint32_t w, h;
		void* data = Walnut::Image::Decode(g_RKey.data(), g_RKey.size(), w, h);
		keyboardIcons.insert({ ImGuiKey_R, std::make_unique<Walnut::Image>(w, h, Walnut::ImageFormat::RGBA, data) });
	}
}