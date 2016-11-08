#pragma once
#include <cstdint>
#include "Util.h"
#include "D2Pool.h"

typedef uint64_t MyGameTimeStamp;

class MyGameTime
{
private:
	uint64_t mLastUpdate;
	MyGameTimeStamp mValue;
	int mDayDuration;
	int mHourDuration;
	int mMinuteDuration;
public:
	static int TICKS_PER_SECOND;

	MyGameTime();
	~MyGameTime();

	void SetValue(MyGameTimeStamp value);
	void SetValue(MyGameTimeStamp value, DWORD lastUpdate);

	MyGameTimeStamp GetTimeStamp();
	void Update();
	void Update(DWORD real_time);

	int GetDay();
	uint64_t GetTotalDays();
	int GetHour();
	uint64_t GetTotalHours();
	int GetMinute();
	uint64_t GetTotalMinutes();
	int GetSecond();
	uint64_t GetTotalSeconds();

	std::wstring ToString();
};

