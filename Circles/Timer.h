#pragma once

class Timer
{
public:
	Timer();
	double GetTime() const;
protected:
	bool m_bUsePerformanceCounter;
	__int64 m_lFrequency;
	__int64 m_lTimerStart;
	double m_dResolution;
};

