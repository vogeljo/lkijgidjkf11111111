#include "ITimedAnimation.h"

ITimedAnimation::ITimedAnimation()
{

}

ITimedAnimation::~ITimedAnimation()
{
}

void ITimedAnimation::Start()
{
	mStartTime = ITimedAnimation::GetTime();
	mCallbackFired = false;
}

void ITimedAnimation::Start(float startValue, float endValue, DWORD duration_ms)
{
	return this->Start(startValue, endValue, duration_ms, nullptr);
}

void ITimedAnimation::Start(float startValue, float endValue, DWORD duration_ms, std::function<void()> on_end_callback)
{
	mStartValue = startValue;
	mEndValue = endValue;
	mDuration = duration_ms;
	mCallback = on_end_callback;
	this->Start();
}

void ITimedAnimation::Abort()
{
	mStartTime = mDuration = 0;
}

float ITimedAnimation::GetProgress()
{
	if (IsRunning())
		return (float)(ITimedAnimation::GetTime() - mStartTime) / (float)mDuration;
	else
		return 0.0f;
}

float ITimedAnimation::GetValue()
{
	if (this->IsRunning())
		return this->CalculateValue(this->GetProgress());
	else {
		return mEndValue;
	}
}

bool ITimedAnimation::IsRunning()
{
	bool running = mDuration != 0 && (ITimedAnimation::GetTime() - mStartTime) < mDuration;

	if (!running && !mCallbackFired) {
		if (mCallback)
			mCallback();
		mCallbackFired = true;
	}

	return running;
}

DWORD ITimedAnimation::GetTime()
{
	return GetTickCount();
}
