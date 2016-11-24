#pragma once
#include "ConstantDynamic.h"

#include <algorithm>

template<typename T>
class BoundedConstantDynamic
	: public ConstantDynamic<T> {
private:
	T mMin, mMax;
public:
	using ConstantDynamic::operator=;

	BoundedConstantDynamic(T value, T min, T max)
		: ConstantDynamic(value), mMin(min), mMax(max)
	{
		this->Set(value);
	}


	/*BoundedConstantDynamic(T value)
		: BoundedConstantDynamic(value, 0, 0)
	{

	}*/

	BoundedConstantDynamic()
		: BoundedConstantDynamic(0, 0, 0)
	{

	}

	virtual ~BoundedConstantDynamic() {

	}

	void Set(T value) override {
		ConstantDynamic::Set(std::min(std::max(value, mMin), mMax));
	}
};