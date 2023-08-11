#include "Notifications.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_internal.h"
#include "Walnut/ImGui/ImGuiTheme.h"

int CalcNumLines(std::string_view text, float width)
{
	int numLines = 1;
	const char* s = &text.data()[0];
	const char* end = &text.data()[text.size()];
	const char* wrap = s;

	while (wrap < end) {
		wrap = ImGui::GetFont()->CalcWordWrapPositionA(1.0f, s, end, width);
		std::string str = s;
		size_t pos = str.find(wrap);
		if (!pos) {
			pos = str.size();
		}
		std::string substr = str.substr(0, pos);
		size_t newline = substr.find("\n");
		if (newline != substr.npos) {
			s = s + newline + 1;
			numLines++;
			continue;
		}
		else if (wrap == end) {
			break;
		}
		numLines++;
		s = wrap;
	}

	return numLines;
}

static const float maxWidth = 350.0f;

namespace BO3PracticeTool
{
	std::vector<InfoNotification> InfoNotification::s_InfoNotis;

	void InfoNotification::AddInfoNoti(std::string_view title, std::string_view message)
	{
		InfoNotification noti(title, message);
		s_InfoNotis.emplace_back(noti);
	}

	void InfoNotification::RenderInfoNotis()
	{
		if (!s_InfoNotis.size()) {
			return;
		}

		std::chrono::time_point<std::chrono::system_clock> currentTime = std::chrono::system_clock::now();

		const ImVec2 viewportPos = ImGui::GetMainViewport()->Pos;
		const ImVec2 viewportSize = ImGui::GetMainViewport()->Size;
		const ImVec2 max = viewportPos + viewportSize;
		const float startX = viewportPos.x + viewportSize.x - ImGui::GetStyle().ItemSpacing.x - maxWidth - 40.0f;
		const float startY = viewportPos.y + 58.0f;
		float previousHeight = 0.0f;
		
		ImGui::PushStyleColor(ImGuiCol_WindowBg, Walnut::UI::Colors::ColorWithMultipliedValue(Walnut::UI::Colors::Theme::titlebar, 1.8f));
		ImGui::PushStyleColor(ImGuiCol_TitleBg, Walnut::UI::Colors::ColorWithMultipliedValue(Walnut::UI::Colors::Theme::titlebar, 1.5f));

		for (size_t i = 0; i < s_InfoNotis.size(); i++) {
			if (currentTime >= s_InfoNotis[i].m_KillTime || !s_InfoNotis[i].m_Opened) {
				s_InfoNotis.erase(s_InfoNotis.begin() + i);
				continue;
			}

			InfoNotification& noti = s_InfoNotis[i];
			float x = startX;
			float y = startY + previousHeight + (ImGui::GetStyle().ItemSpacing.y * i);
			ImVec2 textData = ImGui::CalcTextSize(noti.m_Message.c_str());
			textData = ImGui::CalcTextSize(&noti.m_Message[0], &noti.m_Message[noti.m_Message.size()], false, std::max(maxWidth, textData.x) - (ImGui::GetStyle().WindowPadding.x * 2));
			const float width = std::min(textData.x + 30.0f, maxWidth);
			float height = textData.y + 35.0f + 42.0f;

			ImVec2 windowPos = ImVec2(x + maxWidth - width + 20.0f, y);
			ImVec2 windowSize = ImVec2(width, height);
			if (windowPos.y + windowSize.y >= max.y) {
				continue;
			}

			ImGui::SetNextWindowPos(windowPos);
			ImGui::SetNextWindowSize(windowSize);

			if (ImGui::Begin(noti.m_Tile.c_str(), &noti.m_Opened, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse)) {
				previousHeight += ImGui::GetWindowHeight();

				std::chrono::duration<float> timeRemaining = noti.m_KillTime - currentTime;
				float progress = (timeRemaining.count() / 20.0f) / 1.0f;
				ImGui::ProgressBar(progress, ImVec2(maxWidth, 5), "");
				ImGui::TextWrapped(noti.m_Message.c_str());

				ImGui::End();
			}

			if (i != s_InfoNotis.size() - 1) {
				ImGui::SameLine();
			}
		}

		ImGui::PopStyleColor(2);
	}
}