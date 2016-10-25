#include "Unit.h"

Unit::Unit()
{
	mColor = D2D1::ColorF(rand());
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

UnitLocation Unit::GetLocation()
{
	return mLocation;
}

D2D1_COLOR_F Unit::GetColor()
{
	return mColor;
}

void Unit::SetColor(D2D1_COLOR_F& color)
{
	mColor = color;
}

void Unit::SetLocation(float x, float y)
{
	mLocation.x = max(0.0f, x);
	mLocation.y = max(0.0f, y);
}

Inventory& Unit::GetInventory()
{
	return mInventory;
}

Money Unit::GetMoneyChange()
{
	// NOT IMPLEMENTED
	//return std::rand() % 1000 - 500;
	return 0;
}

UnitLocation::UnitLocation(float x, float y)
	: x(x), y(y)
{

}

UnitLocation::UnitLocation()
	: UnitLocation(0.0f, 0.0f)
{

}

UnitLocation UnitLocation::operator+(UnitLocation& l2)
{
	return UnitLocation(x + l2.x, y + l2.y);
}

UnitLocation UnitLocation::operator-(UnitLocation& l2)
{
	return UnitLocation(x * l2.x, y * l2.y);
}

UnitLocation UnitLocation::operator*(UnitLocation& l2)
{
	return UnitLocation(x - l2.x, y - l2.y);
}
