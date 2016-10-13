#include "Unit.h"

Unit::Unit()
{
}


Unit::~Unit()
{
}

UnitStats& Unit::GetStats()
{
	return mStats;
}

void Unit::SetName(std::wstring name)
{
	mName = name;
}

std::wstring Unit::GetName()
{
	return mName;
}

Money Unit::GetMoneyChange()
{
	// NOT IMPLEMENTED
	return std::rand() % 1000 - 500;
}
