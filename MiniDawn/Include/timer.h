#ifndef _TIMER_H_
#define _TIMER_H_
// Probably worth looking as a platform independent solution, or fall back Boost?
#include <windows.h>

class Timer
{
public:
	bool Initialize();
	void Frame();

	float GetTime();

private:
	INT64 m_frequency;
	float m_ticksPerMs;
	INT64 m_startTime;
	float m_frameTime;
};

#endif