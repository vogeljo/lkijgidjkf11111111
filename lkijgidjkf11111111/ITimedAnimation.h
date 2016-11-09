#pragma once
#include "native.h"
#include <functional>
#include "Util.h"

class ITimedAnimation abstract
{
protected:
	uint64_t mDuration;
	float mStartValue, mEndValue;
	bool mCallbackFired;
	std::function<void()> mCallback;

	uint64_t mStartTime;
public:
	ITimedAnimation();
	virtual ~ITimedAnimation();

	void Start();
	void Start(float startValue, float endValue, DWORD duration_ms);
	void Start(float startValue, float endValue, DWORD duration_ms, std::function<void()> on_end_callback);
	void Abort();

	float GetProgress();
	float GetValue();
	void Set(float value);
	virtual float CalculateValue(float progress) = 0;

	bool IsRunning();
	bool IsRelaxing();
};

