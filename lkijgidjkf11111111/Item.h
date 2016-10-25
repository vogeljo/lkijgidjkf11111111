#pragma once
#include <string>
#include <map>
#include "crc32.h"
#include <d2d1.h>
#include "D2Pool.h"

typedef uint32_t ItemToken;

class Item
{
private:
	std::wstring mName;
	std::wstring mID;
	ItemToken mToken;
	D2Graphic mGraphic;

	void SetID(std::wstring id);
public:
	Item(std::wstring id, std::wstring name, D2Graphic graphic);
	virtual ~Item();

	ItemToken GetToken();
	std::wstring GetName();
	std::wstring GetID();
	D2Graphic GetGraphic();

	operator ItemToken();

	static ItemToken Create(std::wstring id, std::wstring name, D2Graphic graphic);
};

class ItemPool abstract
{
	typedef std::map<ItemToken, Item*> Map;
private:
	static Map sPool;
public:
	static ItemToken Add(Item *item);
	static Item* Get(ItemToken token);
};