#include "Util.h"

bool Util::IsKeyDown(int key)
{
	return GetKeyState(key) & 0x8000;
}

int Util::GetScreenWidth()
{
	return GetSystemMetrics(SM_CXSCREEN);
}

int Util::GetScreenHeight()
{
	return GetSystemMetrics(SM_CYSCREEN);
}
