#pragma once
#include <cstdint>
#include "Util.h"
#include "D2Pool.h"

typedef DWORD MyGameTimeStamp;

class MyGameTime
{
private:
	DWORD mLastUpdate;
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
	int GetTotalDays();
	int GetHour();
	int GetTotalHours();
	int GetMinute();
	int GetTotalMinutes();
	int GetSecond();
	int GetTotalSeconds();

	std::wstring ToString();
};

