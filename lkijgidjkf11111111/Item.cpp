#include "Item.h"

Item ITEM_UNKNOWN = Item(L"unknown", L"Unbekannt", D2Graphic::APPLE);

Item::Item(std::wstring id, std::wstring name, D2Graphic graphic)
	: mName(name), mGraphic(graphic)
{
	this->SetID(id);
}

Item::~Item()
{
}

void Item::SetID(std::wstring id)
{
	for (auto it = id.begin(); it != id.end(); ++it) {
		*it = ::towlower(*it);
	}
	mID = id;
	mToken = ItemToken(crc32(mID));
}

ItemToken Item::GetToken()
{
	return mToken;
}

std::wstring Item::GetName()
{
	return mName;
}

std::wstring Item::GetID()
{
	return mID;
}

D2Graphic Item::GetGraphic()
{
	return mGraphic;
}

Item::operator ItemToken()
{
	return this->GetToken();
}

ItemToken Item::Create(std::wstring id, std::wstring name, D2Graphic graphic)
{
	return ItemPool::Add(new Item(id, name, D2Graphic::APPLE));
}

// ItemPool

ItemPool::Map ItemPool::sPool = ItemPool::Map();

ItemToken ItemPool::Add(Item *item)
{
	ItemToken token = item->GetToken();
	sPool[token]= item;
	return token;
}

Item* ItemPool::Get(ItemToken token)
{
	auto ptr = sPool[token];
	if (!ptr)
		return &ITEM_UNKNOWN;
	else
		return sPool[token];
}
