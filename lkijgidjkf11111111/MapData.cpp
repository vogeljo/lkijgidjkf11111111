#include "MapData.h"

MapData::MapData()
	: MapData(MAP_DEFAULT_WIDTH, MAP_DEFAULT_HEIGHT)
{
}

MapData::MapData(int width, int height)
	: mWidth(width), mHeight(height)
{

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

void MapData::SetType(MapCoordPair cp, MapTileType type)
{
	mValues[cp].type = type;
	this->InvokeChange(cp);
}

MapTileType MapData::GetType(MapCoordPair cp)
{
	auto it = mValues.find(cp);
	if (it != mValues.end()) {
		return it->second.type;
	}
	return MapTileType::None;
}

void MapData::Remove(MapCoordPair cp)
{
	mValues.erase(cp);
}

void MapData::AddChangeCallback(MapDataChangeCallback cb)
{
	mChangeCallbacks.push_back(cb);
}

void MapData::RemoveChangeCallback(MapDataChangeCallback cb)
{
	throw std::exception("Not implemented.");
}
