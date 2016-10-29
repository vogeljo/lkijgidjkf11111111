#pragma once
#include "native.h"

class Util abstract
{
public:
	static bool IsKeyDown(int key);
	static int GetScreenWidth();
	static int GetScreenHeight();

	static DWORD GetTime();
};

