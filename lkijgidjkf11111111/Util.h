#pragma once
#include "native.h"
#include <chrono>

typedef uint64_t TimeStamp;

class Util abstract
{
public:
	static bool IsKeyDown(int key);
	static int GetScreenWidth();
	static int GetScreenHeight();

	// Gets steady, monotonic time in milliseconds.
	static TimeStamp GetTime();

	static D2D1_COLOR_F TweenHealth(float progress);
};

//TODO: implementations to source file (VC doesnt like that; C2676)

static D2D1_RECT_F operator +(D2D1_RECT_F& origin, float add) {
	return D2D1::RectF(origin.left + add, origin.top + add, origin.right + add, origin.bottom + add);
}
static D2D1_RECT_F operator +(const D2D1_RECT_F origin, const D2D1_POINT_2F add) {
	return D2D1::RectF(origin.left + add.x, origin.top + add.y, origin.right + add.x, origin.bottom + add.y);
}