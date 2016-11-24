#pragma once
#include <set>
#include <algorithm>
#include "native.h"
#include "MapData.h"
#include "D2Pool.h"
#include "Unit.h"

class HouseInfo
{
private:
	MapData& mMap;
	std::wstring mName;
	std::set<MapCoordPair> mIndices;
	MapCoord mMinX;
	MapCoord mMaxX;
	MapCoord mMinY;
	MapCoord mMaxY;

	ID2D1Brush *mBrush;

	void OnChanged();
public:
	HouseInfo(MapData& map, std::wstring name);
	virtual ~HouseInfo();

	std::wstring GetName();
	void SetName(std::wstring value);
	bool IsEmpty();

	void AddField(MapCoordPair& cp);
	void RemoveField(MapCoordPair& cp);
	bool Intersects(MapCoordPair& cp);
	bool Intersects(MapCoord x, MapCoord y);
	bool Intersects(Unit& unit);

	MapCoordPair GetTopLeftCorner();
	MapCoordPair GetBottomRightCorner();

	MapCoordPair GetCenter();

	void SetBrush(ID2D1Brush *value);
	ID2D1Brush* GetBrush();

	size_t GetFieldCount();
};

