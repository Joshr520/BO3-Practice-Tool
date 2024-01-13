#pragma once

#include <unordered_map>
#include <string>

typedef enum Events : uint8_t {
	Active, Paused, Terminate
} NotifyEvent;

class Notify {
public:
	Notify(std::string_view id) : m_ID(id) { }

	static bool Terminated(std::string_view id);
	static bool Running(std::string_view id);
	bool Terminated();
	bool Running();

	static void End(std::string_view id);
	static void Pause(std::string_view id);
	static void Resume(std::string_view id);
private:
	std::string m_ID;
	NotifyEvent m_Status = Active;
	static std::unordered_map<std::string, NotifyEvent> s_Notifies;
};