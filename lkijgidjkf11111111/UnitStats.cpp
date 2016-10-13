#include "UnitStats.h"



UnitStats::UnitStats()
{
}


UnitStats::~UnitStats()
{
}

int UnitStats::Get(Stat stat)
{
	return mValues[stat];
}

int& UnitStats::GetRef(Stat stat)
{
	return mValues[stat];
}

void UnitStats::Set(Stat stat, int value)
{
	mValues[stat] = value;
}

void UnitStats::Add(Stat stat, int value)
{
	mValues[stat] += value;
}

void UnitStats::Substract(Stat stat, int value)
{
	mValues[stat] -= value;
}

void UnitStats::ForEach(std::function<void(Stat, int)> callback)
{
	for each (auto pair in mValues) {
		callback(pair.first, pair.second);
	}
}
