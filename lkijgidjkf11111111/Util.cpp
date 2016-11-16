#include "Util.h"

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
	return std::chrono::steady_clock::now().time_since_epoch().count() / 1000000;
	//return GetTickCount64();
}

D2D1_COLOR_F Util::TweenHealth(float progress)
{
	return D2D1::ColorF(1.0f - progress, progress, 0.0f);
}
