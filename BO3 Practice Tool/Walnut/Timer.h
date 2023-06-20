#pragma once

#include <iostream>
#include <string>
#include <chrono>

namespace Walnut {

	class Timer
	{
	public:
		Timer()
		{
			Reset();
		}

		void Reset()
		{
			m_Start = std::chrono::high_resolution_clock::now();
		}

		int Started()
		{
			return static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(m_Start.time_since_epoch()).count());
		}

		int Elapsed()
		{
			return static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - m_Start).count());
		}

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
	};

	class ScopedTimer
	{
	public:
		ScopedTimer(const std::string& name)
			: m_Name(name) {}
		~ScopedTimer()
		{
			int time = m_Timer.Elapsed();
			std::cout << "[TIMER] " << m_Name << " - " << time << "ms\n";
		}
	private:
		std::string m_Name;
		Timer m_Timer;
	};



}
