#pragma once

#include <string>
#include <chrono>

namespace BO3PracticeTool
{
	class InfoNotification {
	public:
		InfoNotification(std::string_view title, std::string_view message)
			: m_Tile(std::format("{}##{}", title, s_InfoNotis.size())), m_Message(message), m_Created(std::chrono::system_clock::now())
		{
			m_KillTime = m_Created + std::chrono::seconds(20);
		}

		static void AddInfoNoti(std::string_view title, std::string_view message);
		static void RenderInfoNotis();
	private:
		bool m_Opened = true;
		std::string m_Tile;
		std::string m_Message;
		std::chrono::time_point<std::chrono::system_clock> m_Created;
		std::chrono::time_point<std::chrono::system_clock> m_KillTime;

		static std::vector<InfoNotification> s_InfoNotis;
	};
}