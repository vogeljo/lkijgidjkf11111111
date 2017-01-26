#pragma once
#include "native.h"
#include <chrono>
#include <stack>
#include <algorithm>

typedef uint64_t TimeStamp;
typedef TimeStamp TimeDiff;

class Util abstract
{
public:
	static bool IsKeyDown(int key);
	static int GetScreenWidth();
	static int GetScreenHeight();

	// Gets steady, monotonic time in milliseconds.
	static TimeStamp GetTime();
	static TimeStamp GetRealTime();
	static TimeStamp PushTime();
	static TimeStamp PopTime();

	static D2D1_COLOR_F TweenHealth(float progress);

	static std::wstring ToLower(std::wstring input);
	static std::wstring ToUpper(std::wstring input);

	template<typename T>
	static std::wstring Concat(T begin, T end, std::wstring delimeter = L" ") {
		std::wstring result;
		auto len = std::distance(begin, end);
		for (auto it = begin; it != end; ++it) {
			auto val = *it;
			result += val;
			if (len != 1)
				result += delimeter;
			--len;
		}
		return result;
	}
};

//TODO: implementations to source file (VC doesnt like that; C2676)

static D2D1_RECT_F operator +(D2D1_RECT_F& origin, float add) {
	return D2D1::RectF(origin.left + add, origin.top + add, origin.right + add, origin.bottom + add);
}
static D2D1_RECT_F operator +(const D2D1_RECT_F origin, const D2D1_POINT_2F add) {
	return D2D1::RectF(origin.left + add.x, origin.top + add.y, origin.right + add.x, origin.bottom + add.y);
}