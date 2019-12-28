#pragma once
#include <windows.h>

//精确到微秒之后的延时类（基于多媒体定时器）
//以后直接调用myTimer.Sleep(n);

class myTimer
{
private:
	static LARGE_INTEGER m_clk;			// 保存时钟信息
	static LONGLONG m_oldclk;			// 保存开始时钟和结束时钟
	static int m_freq;					// 时钟频率(时钟时间换算率)，时间差

public:
	static void Sleep(int ms);
};

LARGE_INTEGER myTimer::m_clk;
LONGLONG myTimer::m_oldclk;
int myTimer::m_freq = 0;

// 延时
void myTimer::Sleep(int ms)
{
	if (m_oldclk == 0)
	{
		QueryPerformanceFrequency(&m_clk);
		m_freq = (int)m_clk.QuadPart / 1000;	// 获得计数器的时钟频率

												// 开始计时
		QueryPerformanceCounter(&m_clk);
		m_oldclk = m_clk.QuadPart;				// 获得开始时钟
	}

	unsigned int c = ms * m_freq;

	m_oldclk += c;

	QueryPerformanceCounter(&m_clk);

	if (m_clk.QuadPart > m_oldclk)
		m_oldclk = m_clk.QuadPart;
	else
		do
		{
			::Sleep(1);
			QueryPerformanceCounter(&m_clk);	// 获得终止时钟
		} while (m_clk.QuadPart < m_oldclk);
}
