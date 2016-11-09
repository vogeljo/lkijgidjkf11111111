#pragma once
#include "native.h"
#include <chrono>

class Util abstract
{
public:
	static bool IsKeyDown(int key);
	static int GetScreenWidth();
	static int GetScreenHeight();

	// Gets steady, monotonic time in milliseconds.
	static uint64_t GetTime();

	static D2D1_COLOR_F TweenHealth(float progress);
};

