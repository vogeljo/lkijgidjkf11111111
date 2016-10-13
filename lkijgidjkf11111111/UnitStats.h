#pragma once
#include <map>
#include <functional>

enum class Stat {
	Health, Money
};

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

