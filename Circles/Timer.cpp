#include "stdafx.h"
#include "Timer.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

Timer::Timer()
{
	if (QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&m_lFrequency)))
    {
		m_bUsePerformanceCounter = true;
		m_dResolution = 1.0 / m_lFrequency;
        QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&m_lTimerStart));
    }
    else
    {
		// high precision timer is anavailable
		m_bUsePerformanceCounter = false;
        m_dResolution = 0.001; // winmm resolution is 1 ms
		m_lTimerStart = timeGetTime();
    }
}

double Timer::GetTime() const
{
	__int64 t;
	if (m_bUsePerformanceCounter)
	{
		__int64 elapsedTime;
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&elapsedTime));
		t = elapsedTime - m_lTimerStart;
	}
	else
		t = timeGetTime() - m_lTimerStart;
	
	return t * m_dResolution;
}