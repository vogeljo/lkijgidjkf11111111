#pragma once
#include <numeric>

template<typename T>
class IDynamic abstract {
	static_assert(std::is_arithmetic<T>::value, "IDynamic templates must have an arithmetic type.");

public:
	IDynamic() {

	}

	virtual ~IDynamic() {
	}

	virtual T Get() = 0;
};

typedef IDynamic<float> IDynamicF;