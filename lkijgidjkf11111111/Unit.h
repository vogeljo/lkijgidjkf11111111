#pragma once
#include <string>
#include "UnitStats.h"

typedef int Money;

class Unit
{
protected:
	UnitStats mStats;
	std::wstring mName;
public:
	Unit();
	virtual ~Unit();

	UnitStats& GetStats();

	void SetName(std::wstring name);
	std::wstring GetName();

	Money GetMoneyChange();
};

