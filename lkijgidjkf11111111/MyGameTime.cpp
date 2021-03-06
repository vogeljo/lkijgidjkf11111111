#include "MyGameTime.h"

#include <cstdio>

// TICKS_PER_SECOND * sec(reality) = sec(game)
int MyGameTime::TICKS_PER_SECOND = 360;

std::wstring ZeroLeft(std::wstring& src, int digits) {
	std::wstring res = src;
	while (res.length() < digits)
		res.insert(res.begin(), L'0');
	return res;
}

MyGameTime::MyGameTime()
	: mLastUpdate(Util::GetTime()), mMinuteDuration(60), mHourDuration(60), mDayDuration(24)
{
}

MyGameTime::~MyGameTime()
{
}

void MyGameTime::SetValue(MyGameTimeStamp value)
{
	this->SetValue(value, Util::GetTime());
}

void MyGameTime::SetValue(MyGameTimeStamp value, DWORD lastUpdate)
{
	mValue = value;
	mLastUpdate = lastUpdate;
}

MyGameTimeStamp MyGameTime::GetTimeStamp()
{
	return mValue;
}

void MyGameTime::Update()
{
	this->Update(Util::GetTime());
}

void MyGameTime::Update(DWORD real_time)
{
	if (mLastUpdate) {
		auto diff = real_time - mLastUpdate;
		mValue += diff;
	}

	mLastUpdate = real_time;
}

int MyGameTime::GetDay()
{
	return this->GetTotalDays() + 1;
}

uint64_t MyGameTime::GetTotalDays()
{
	return this->GetTotalHours() / mDayDuration;
}

int MyGameTime::GetHour()
{
	return this->GetTotalHours() % mDayDuration;
}

uint64_t MyGameTime::GetTotalHours()
{
	return this->GetTotalMinutes() / mHourDuration;
}

int MyGameTime::GetMinute()
{
	return this->GetTotalMinutes() % mHourDuration;
}

uint64_t MyGameTime::GetTotalMinutes()
{
	return this->GetTotalSeconds() / mMinuteDuration;
}

int MyGameTime::GetSecond()
{
	return this->GetTotalSeconds() % mMinuteDuration;
}

uint64_t MyGameTime::GetTotalSeconds()
{
	return mValue * TICKS_PER_SECOND / 1000;
}

std::wstring MyGameTime::ToString()
{
	std::wstring str = L"";
	//str += L"Tag ";
	//str += std::to_wstring(this->GetDay());
	//str += L" - ";
	str += ZeroLeft(std::to_wstring(this->GetHour()), 2);
	str += L":";
	str += ZeroLeft(std::to_wstring(this->GetMinute()), 2);
	//str += L":";
	//str += ZeroLeft(std::to_wstring(this->GetSecond()), 2);
	return str;
}
