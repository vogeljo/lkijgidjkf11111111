#include "Unit.h"

UnitLocation UnitLocation::NONE = UnitLocation::UnitLocation(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());

Unit::Unit()
	: mSpeed(5.0f), mVisionRange(25.0f), mWeapon(nullptr)
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
	mLocation.x = std::max(0.0f, x);
	mLocation.y = std::max(0.0f, y);
}

Inventory& Unit::GetInventory()
{
	return mInventory;
}

float Unit::GetVisionRange()
{
	return mVisionRange;
}

void Unit::SetVisionRange(float value)
{
	mVisionRange = value;
}

bool Unit::HasVisionAt(UnitLocation & loc)
{
	auto diff = loc.DistanceTo(this->GetLocation());
	return diff <= this->GetVisionRange();
}

bool Unit::HasVisionAt(Unit & unit)
{
	return this->HasVisionAt(unit.GetLocation());
}

bool Unit::HasVisionAt(MapCoordPair & pair)
{
	return this->HasVisionAt(UnitLocation(pair.x + 0.5f, pair.y + 0.5f));
}

float Unit::GetVisionIntensityAt(UnitLocation & loc)
{
	float diff = this->GetLocation().DistanceTo(loc);
	float rel = diff / mVisionRange;

	if (this->HasVisionAt(loc))
		return std::sqrtf(mVisionRange - diff) / std::sqrtf(mVisionRange - 0);
	else
		return 0.0f;
}

void Unit::SetSpeed(float value)
{
	mSpeed = value;
}

float Unit::GetSpeed()
{
	return mSpeed;
}

IWeapon* Unit::GetWeapon()
{
	return mWeapon;
}

void Unit::SetWeapon(IWeapon* w)
{
	mWeapon = w;
}

Money Unit::GetMoneyChange()
{
	// NOT IMPLEMENTED
	//return std::rand() % 1000 - 500;
	return 0;
}

void Unit::Update()
{
	auto val = Util::GetTime();
	this->OnUpdate(mLastUpdate != 0 ? val - mLastUpdate : 0);
	mLastUpdate = val;
}

void Unit::OnUpdate(uint64_t diff_ms)
{

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
	return UnitLocation(x - l2.x, y - l2.y);
}

UnitLocation UnitLocation::operator*(UnitLocation& l2)
{
	return UnitLocation(x * l2.x, y * l2.y);
}

bool UnitLocation::operator==(UnitLocation& l2)
{
	return this->x == l2.x && this->y == l2.y;
}

bool UnitLocation::operator!=(UnitLocation & l2)
{
	return !(this->operator==(l2));
}

bool UnitLocation::IsNone()
{
	return *this == UnitLocation::NONE;
}

float UnitLocation::DistanceTo(UnitLocation& loc)
{
	return sqrt(pow(loc.x - this->x, 2) + pow(loc.y - this->y, 2));
}