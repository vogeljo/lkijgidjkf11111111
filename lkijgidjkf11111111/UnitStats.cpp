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
	mValues[stat] = std::max(UNIT_STAT_MIN, std::min(UNIT_STAT_MAX, value));
}

void UnitStats::Add(Stat stat, int value)
{
	this->Set(stat, this->Get(stat) + value);
}

void UnitStats::Substract(Stat stat, int value)
{
	this->Set(stat, this->Get(stat) - value);
}

void UnitStats::ForEach(std::function<void(Stat, int)> callback)
{
	for each (auto pair in mValues) {
		callback(pair.first, pair.second);
	}
}

std::wstring GetStatText(Stat s)
{
	std::wstring str = L"<unknown>";
	switch (s) {
	case Stat::Health:
		str = L"Gesundheit";
		break;
	case Stat::Money:
		str = L"Geld in \x00A2";
		break;
	}
	return str;
}
