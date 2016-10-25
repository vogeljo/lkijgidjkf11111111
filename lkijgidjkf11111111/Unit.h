#pragma once
#include <string>
#include <d2d1.h>
#include "UnitStats.h"
#include "Inventory.h"

typedef int Money;

struct UnitLocation {
	float x, y;

public:
	UnitLocation(float x, float y);
	UnitLocation();

	UnitLocation operator+(UnitLocation& l2);
	UnitLocation operator-(UnitLocation& l2);
	UnitLocation operator*(UnitLocation& l2);
};

class Unit
{
protected:
	UnitStats mStats;
	std::wstring mName;
	UnitLocation mLocation;

	D2D1_COLOR_F mColor;
	Inventory mInventory;
public:
	Unit();
	virtual ~Unit();

	UnitStats& GetStats();

	void SetName(std::wstring name);
	std::wstring GetName();
	UnitLocation GetLocation();
	D2D1_COLOR_F GetColor();
	void SetColor(D2D1_COLOR_F& color);
	void SetLocation(float x, float y);
	Inventory& GetInventory();

	Money GetMoneyChange();
};

