#pragma once
#include "MapTile.h"
#include <map>
#include <vector>
#include <functional>
#include <fstream>
#include <iostream>
#include <cstdint>

#define MAP_DEFAULT_WIDTH 500
#define MAP_DEFAULT_HEIGHT 500

typedef uint32_t MapCoord;
typedef uint64_t MapCoordIndex;

struct MapCoordPair {
	MapCoord x, y;

	MapCoordPair();
	MapCoordPair(MapCoord x, MapCoord y);
	
	MapCoordIndex ToIndex(MapCoord width);

	friend bool operator<(const MapCoordPair& mcp1, const MapCoordPair& mcp2);
};
//typedef uint64_t MapCoordPair;

bool operator<(const MapCoordPair& mcp1, const MapCoordPair& mcp2);

typedef std::function<void(MapCoordPair)> MapDataChangeCallback;

class MapData
{
private:
	int mWidth, mHeight;
	//std::map<MapCoordPair, MapTile> mValues;
	std::vector<MapDataChangeCallback> mChangeCallbacks;

	MapTileType *data;

	void InvokeChange(MapCoordPair cp);
protected:
	MapCoordIndex CoordPairToIndex(MapCoordPair& pair);
	MapCoordPair CoordIndexToPair(MapCoordIndex index);
	void SetType(MapCoordIndex index, MapTileType type);
	MapTileType GetType(MapCoordIndex index);
	MapCoordIndex GetDataLength();
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

	void AddChangeCallback(MapDataChangeCallback cb);
	void RemoveChangeCallback(MapDataChangeCallback cb);

	static bool FromFile(std::string filename, MapData **dPtr);
};