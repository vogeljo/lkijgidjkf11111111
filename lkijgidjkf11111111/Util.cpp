#include "Util.h"

struct {
	std::stack<TimeStamp> stamps;
} TIME_DATA;

bool Util::IsKeyDown(int key)
{
	return !!(GetKeyState(key) & 0x8000);
}

int Util::GetScreenWidth()
{
	return GetSystemMetrics(SM_CXSCREEN);
}

int Util::GetScreenHeight()
{
	return GetSystemMetrics(SM_CYSCREEN);
}

TimeStamp Util::GetTime()
{
	if (TIME_DATA.stamps.empty())
		return GetRealTime();
	else
		return TIME_DATA.stamps.top();
}

TimeStamp Util::GetRealTime()
{
	return std::chrono::steady_clock::now().time_since_epoch().count() / 1000000;
}

TimeStamp Util::PushTime()
{
	auto rt = Util::GetRealTime();
	TIME_DATA.stamps.push(rt);
	return rt;
}

TimeStamp Util::PopTime()
{
	if (TIME_DATA.stamps.empty())
		return TimeStamp();

	auto t = TIME_DATA.stamps.top();
	TIME_DATA.stamps.pop();
	return t;
}

D2D1_COLOR_F Util::TweenHealth(float progress)
{
	return D2D1::ColorF(1.0f - progress, progress, 0.0f);
}
