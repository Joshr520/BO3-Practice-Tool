#include "Notify.h"

std::unordered_map<std::string, NotifyEvent> Notify::s_Notifies;

bool Notify::Terminated(std::string_view id)
{
	auto it = s_Notifies.find(id.data());
	if (it == s_Notifies.end()) {
		return true;
	}
	return it->second == Terminate;
}

bool Notify::Running(std::string_view id)
{
	auto it = s_Notifies.find(id.data());
	if (it == s_Notifies.end()) {
		return true;
	}
	return it->second == Active;
}

bool Notify::Terminated()
{
	return m_Status == Terminate;
}

bool Notify::Running()
{
	return m_Status == Active;
}

void Notify::End(std::string_view id)
{
	auto it = s_Notifies.find(id.data());
	if (it == s_Notifies.end()) {
		it->second = Terminate;
	}
}

void Notify::Pause(std::string_view id)
{
	auto it = s_Notifies.find(id.data());
	if (it == s_Notifies.end()) {
		it->second = Paused;
	}
}

void Notify::Resume(std::string_view id)
{
	auto it = s_Notifies.find(id.data());
	if (it == s_Notifies.end()) {
		it->second = Active;
	}
}