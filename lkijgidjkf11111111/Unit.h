#pragma once
#define _USE_MATH_DEFINES

#include <string>
#include <algorithm>
#include <cmath>
#include "UnitStats.h"
#include "Inventory.h"
#include "Util.h"
#include "MapData.h"

typedef int Money;

struct UnitLocation {
	float x, y;

public:
	UnitLocation(float x, float y);
	UnitLocation();

	UnitLocation operator+(UnitLocation& l2);
	UnitLocation operator-(UnitLocation& l2);
	UnitLocation operator*(UnitLocation& l2);
	bool operator==(UnitLocation& l2);
	bool IsNone();
	float DistanceTo(UnitLocation& loc);

	static UnitLocation NONE;
};

class Unit
{
protected:
	UnitStats mStats;
	std::wstring mName;
	UnitLocation mLocation;
	float mSpeed;
	float mVisionRange;

	D2D1_COLOR_F mColor;
	Inventory mInventory;

	uint64_t mLastUpdate;

	virtual void OnUpdate(uint64_t diff_ms);
public:
	Unit();
	virtual ~Unit();

	UnitStats& GetStats();

	void SetName(std::wstring name);
	std::wstring GetName();
	virtual UnitLocation GetLocation();
	D2D1_COLOR_F GetColor();
	void SetColor(D2D1_COLOR_F& color);
	void SetLocation(float x, float y);
	Inventory& GetInventory();

	float GetVisionRange();
	void SetVisionRange(float value);
	bool HasVisionAt(UnitLocation& loc);
	bool HasVisionAt(Unit& unit);
	bool HasVisionAt(MapCoordPair& pair);
	float GetVisionIntensityAt(UnitLocation& loc);

	void SetSpeed(float value);
	float GetSpeed();

	Money GetMoneyChange();

	void Update();
};