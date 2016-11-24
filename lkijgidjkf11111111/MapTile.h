#pragma once
#include "native.h"
#include <string>

enum class MapTileType : uint8_t {
	None, Unknown, Water, Dirt, Stone, Sand, House
};

struct MapTile
{
public:
public:
	MapTile();
	~MapTile();

	MapTileType type;
};

D2D1_COLOR_F GetMapTileColor(MapTileType type);
void GetMapTileInfo(MapTileType type, std::wstring& dTitle, std::wstring& dText);

 