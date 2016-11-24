#include "HouseInfo.h"

HouseInfo::HouseInfo(MapData& map, std::wstring name)
	: mMap(map), mName(name), mMinX(std::numeric_limits<MapCoord>::max()), mMinY(std::numeric_limits<MapCoord>::max())
{
	this->SetBrush(D2Pool::GetSolidColorBrush(D2D1::ColorF::Violet));
}

HouseInfo::~HouseInfo()
{
}

std::wstring HouseInfo::GetName()
{
	return mName;
}

void HouseInfo::SetName(std::wstring value)
{
	mName = value;
}

bool HouseInfo::IsEmpty()
{
	return !this->GetFieldCount();
}

void HouseInfo::OnChanged()
{

}

void HouseInfo::AddField(MapCoordPair & cp)
{
	mIndices.insert(cp);
	mMaxX = std::max(mMaxX, cp.x);
	mMaxY = std::max(mMaxY, cp.y);

	mMinX = std::min(mMinX, cp.x);
	mMinY = std::min(mMinY, cp.y);

	mMap.SetType(cp, MapTileType::House);
	mMap.SetData(cp, this);

	this->OnChanged();
}

void HouseInfo::RemoveField(MapCoordPair & cp)
{
	mIndices.erase(cp);

	mMap.SetType(cp, MapTileType::None);
	mMap.SetData(cp, nullptr);

	this->OnChanged();
}

bool HouseInfo::Intersects(MapCoordPair & cp)
{
	return mIndices.find(cp) != mIndices.end();
}

bool HouseInfo::Intersects(MapCoord x, MapCoord y)
{
	return this->Intersects(MapCoordPair(x, y));
}

bool HouseInfo::Intersects(Unit & unit)
{
	auto px = (MapCoord)std::floor(unit.GetLocation().x);
	auto py = (MapCoord)std::floor(unit.GetLocation().y);

	return this->Intersects(px, py);
}

MapCoordPair HouseInfo::GetTopLeftCorner()
{
	return MapCoordPair(mMinX, mMinY);
}

MapCoordPair HouseInfo::GetBottomRightCorner()
{
	return MapCoordPair(mMaxX, mMaxY);
}

MapCoordPair HouseInfo::GetCenter()
{
	return MapCoordPair(mMinX + (mMaxX - mMinX) / 2, mMinY + (mMaxY - mMinY) / 2);
}

void HouseInfo::SetBrush(ID2D1Brush *value)
{
	mBrush = value;
}

ID2D1Brush* HouseInfo::GetBrush()
{
	return mBrush;
}

size_t HouseInfo::GetFieldCount()
{
	return mIndices.size();
}
