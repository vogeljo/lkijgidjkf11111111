#pragma once
#include <map>
#include <functional>
#include <algorithm>
#include <limits>

enum class Stat {
	Health, Money
};

std::wstring GetStatText(Stat s);

#define UNIT_STAT_MAX 1000
#define UNIT_STAT_MIN 0

class UnitStats
{
private:
	std::map<Stat, int> mValues;
public:
	UnitStats();
	~UnitStats();

	int Get(Stat stat);
	int& GetRef(Stat stat);
	void Set(Stat stat, int value);

	void Add(Stat stat, int value);
	void Substract(Stat stat, int value);

	void ForEach(std::function<void(Stat, int)> callback);
};

