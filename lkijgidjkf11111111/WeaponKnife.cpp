#include "WeaponKnife.h"
#include "Unit.h"

WeaponKnife::~WeaponKnife()
{
}

D2D1_RECT_F WeaponKnife::GetBounds()
{
	auto wp_pos = mOwner->GetLocation();
	return D2D1::RectF(wp_pos.x - 10.0f, wp_pos.y - 5.0f, wp_pos.x, wp_pos.y);
}

float WeaponKnife::GetDamage()
{
	return 7.0f;
}

ID2D1Bitmap* WeaponKnife::GetGraphic()
{
	return D2Pool::GetGraphic(D2Graphic::KNIFE);
}

std::wstring WeaponKnife::GetName()
{
	return L"Messer";
}

bool WeaponKnife::Intersects(UnitLocation& loc)
{
	auto b = this->GetBounds();
	return (b.left <= loc.x && b.top <= loc.y && b.bottom >= loc.y && b.right >= loc.x);
}

void WeaponKnife::OnHit(Unit& unit)
{
	unit.GetStats().Substract(Stat::Health, this->GetDamage());
}
