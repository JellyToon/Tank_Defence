#pragma once

#include <windows.h>

class TimerClass
{
public:
	TimerClass();
	~TimerClass();

	TimerClass(const TimerClass&);

	bool Initialize();
	void Frame();
	
	float GetTime();

	float GetRealTime();

private:

	INT64 m_frequency;
	float m_ticksPerMs;
	INT64 m_startTime;
	float m_frameTime;

	INT64 m_realStartTime;
};

