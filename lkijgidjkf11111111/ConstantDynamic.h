#pragma once
#include "IDynamic.h"

template<typename T>
class ConstantDynamic :
	public IDynamic<T>
{
private:
	T mValue;
public:
	ConstantDynamic(T value) : mValue(value) {

	}

	virtual ~ConstantDynamic() {}


	virtual T Get() override
	{
		return mValue;
	}

	virtual void Set(T value) {
		mValue = value;
	}

	ConstantDynamic<T>& operator=(T value);
};

template<typename T>
ConstantDynamic<T>& ConstantDynamic<T>::operator=(T value) {
	this->Set(value);
	return *this;
}

typedef ConstantDynamic<float> ConstantDynamicF;