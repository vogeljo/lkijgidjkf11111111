#pragma once
#include "Util.h"
#include "ITimedDynamic.h"

template<typename T>
class TweenDynamic abstract
	: public ITimedDynamic<T>
{
protected:
	T mStartValue, mEndValue;
	TimeDiff mDuration;
public:
	TweenDynamic(T start, T end, TimeDiff duration)
	{
		this->Reset(start, end, duration);
	}

	TweenDynamic()
		: TweenDynamic(0, 0, 0)
	{

	}

	virtual ~TweenDynamic() {

	}

	virtual T GetTweenValue(float progress) = 0;

	virtual T Get() override
	{
//		printf("%d\n", this->GetTimeDiff());
		if (this->HasEnded())
			return mEndValue;
		else
			return this->GetTweenValue(std::max(0.0f, std::min(1.0f, (float)this->GetTimeDiff() / (float)mDuration)));
	}

	bool HasEnded() {
		return mDuration == T(0) || this->GetTimeDiff() >= mDuration;
	}

	using ITimedDynamic::Reset;
	void Reset(T start, T end, TimeDiff duration) {
		mStartValue = start;
		mEndValue = end;
		mDuration = duration;
		this->Reset();
	}
};

template<typename T>
class LinearTweenDynamic
	: public TweenDynamic<T>
{

public:
	using TweenDynamic::TweenDynamic;

	virtual ~LinearTweenDynamic() {

	}

	virtual T GetTweenValue(float progress) override
	{
		return mStartValue + progress * (mEndValue - mStartValue);
	}
};