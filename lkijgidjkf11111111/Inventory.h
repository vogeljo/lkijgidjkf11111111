#pragma once
#include <string>
#include <map>
#include <functional>
#include <algorithm>
#include "Item.h"

class Inventory
{
private:
	std::map<ItemToken, int> mItems;
public:
	Inventory();
	~Inventory();

	void Add(ItemToken item, int amount);
	void Remove(ItemToken item);
	void Remove(ItemToken item, int amount);

	int Get(ItemToken item);
	void Set(ItemToken item, int value);

	int GetTotalCount();

	void ForEachItem(std::function<void(ItemToken, int)> callback);
};

