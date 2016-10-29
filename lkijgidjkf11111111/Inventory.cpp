#include "Inventory.h"


Inventory::Inventory()
{
}


Inventory::~Inventory()
{
}

void Inventory::Add(ItemToken item, int amount)
{
	mItems[item] += amount;
}

void Inventory::Remove(ItemToken item)
{
	mItems[item] = 0;
}

void Inventory::Remove(ItemToken item, int amount)
{
	mItems[item] = std::max(0, mItems[item] - amount);
}

int Inventory::Get(ItemToken item)
{
	return mItems[item];
}

void Inventory::Set(ItemToken item, int value)
{
	mItems[item] = value;
}

int Inventory::GetTotalCount()
{
	int sum = 0;
	for each(auto it in mItems) {
		sum += it.second;
	}
	return sum;
}

void Inventory::ForEachItem(std::function<void(ItemToken, int)> callback)
{
	for each (auto it in mItems) {
		callback(it.first, it.second);
	}
}
