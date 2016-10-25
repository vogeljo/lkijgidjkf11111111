#include "MapData.h"

#include <fstream>
#include <iostream>
#include <cstdint>

MapData::MapData()
	: MapData(MAP_DEFAULT_WIDTH, MAP_DEFAULT_HEIGHT)
{
}

MapData::MapData(int width, int height)
	: mWidth(width), mHeight(height)
{
	data = new MapTileType[width * height]{ MapTileType::None };
}

MapData::~MapData()
{
}

void MapData::InvokeChange(MapCoordPair cp)
{
	for each (auto cb in mChangeCallbacks)
		cb(cp);
}

int MapData::GetWidth()
{
	return mWidth;
}

int MapData::GetHeight()
{
	return mHeight;
}

int MapData::GetMinX()
{
	return 0;
}

int MapData::GetMinY()
{
	return 0;
}

MapCoordIndex MapData::CoordPairToIndex(MapCoordPair& pair)
{
	return pair.y * this->GetWidth() + pair.x;
}

MapCoordPair MapData::CoordIndexToPair(MapCoordIndex index)
{
	return MapCoordPair(index % this->GetWidth(), index / this->GetWidth());
}

void MapData::SetType(MapCoordPair cp, MapTileType type)
{
	this->SetType(this->CoordPairToIndex(cp), type);
}

void MapData::SetType(MapCoordIndex index, MapTileType type)
{
	if (index >= 0 && index < this->GetDataLength()) {
		data[index] = type;
		this->InvokeChange(this->CoordIndexToPair(index));
	}
}

MapTileType MapData::GetType(MapCoordPair cp)
{
	return this->GetType(this->CoordPairToIndex(cp));
}

MapTileType MapData::GetType(MapCoordIndex index)
{
	if (index >= 0 && index < this->GetDataLength())
		return data[index];
	else
		return MapTileType::None;
}

MapCoordIndex MapData::GetDataLength()
{
	return this->GetWidth() * this->GetWidth();
}

void MapData::AddChangeCallback(MapDataChangeCallback cb)
{
	mChangeCallbacks.push_back(cb);
}

void MapData::RemoveChangeCallback(MapDataChangeCallback cb)
{
	throw std::exception("Not implemented.");
}

bool MapData::FromFile(std::string filename, MapData **dPtr)
{
	std::ifstream ifs(filename);
	if (ifs.good()) {
		uint32_t width, height;
		ifs.read((char*)&width, sizeof(width));
		ifs.read((char*)&height, sizeof(height));
		MapData *ptr = (*dPtr = new MapData(width, height));

		uint64_t len = width * height;

		for (uint64_t i = 0; i < len; ++i) {
			MapTileType type;
			ifs.read((char*)&type, sizeof(type));
			if (ifs.bad())
				break;
			MapCoordPair pair = MapCoordPair(i % width, i / width);
			ptr->SetType(pair,  type);
		}

		return ifs.good();
	}
	return false;
}

// MapCoordPair

MapCoordPair::MapCoordPair()
	: MapCoordPair(0, 0)
{

}

MapCoordPair::MapCoordPair(MapCoord x, MapCoord y)
	: x(x), y(y)
{

}

MapCoordIndex MapCoordPair::ToIndex(MapCoord width)
{
	return width * y + x;
}

bool operator<(const MapCoordPair& mcp1, const MapCoordPair& mcp2)
{
	if (mcp1.x == mcp2.x)
		return mcp1.y < mcp2.y;
	else
		return mcp1.x < mcp2.x;
}
