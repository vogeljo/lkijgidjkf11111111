#pragma once
#include "MapTile.h"
#include <cstdint>
#include <map>
#include <vector>
#include <functional>

#define MAP_DEFAULT_WIDTH 0xFFFF
#define MAP_DEFAULT_HEIGHT 0xFFFF

typedef uint32_t MapCoord;
typedef uint64_t MapCoordPair;

typedef std::function<void(MapCoordPair)> MapDataChangeCallback;

class MapData
{
private:
	int mWidth, mHeight;
	std::map<MapCoordPair, MapTile> mValues;
	std::vector<MapDataChangeCallback> mChangeCallbacks;

	void InvokeChange(MapCoordPair cp);
public:
	MapData();
	MapData(int width, int height);
	~MapData();

	int GetWidth();
	int GetHeight();

	int GetMinX();
	int GetMinY();

	void SetType(MapCoordPair cp, MapTileType type);
	MapTileType GetType(MapCoordPair cp);

	void Remove(MapCoordPair cp);

	void AddChangeCallback(MapDataChangeCallback cb);
	void RemoveChangeCallback(MapDataChangeCallback cb);
};

