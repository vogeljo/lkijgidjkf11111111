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

};


typedef ConstantDynamic<float> ConstantDynamicF;