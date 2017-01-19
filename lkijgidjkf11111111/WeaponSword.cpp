#include "WeaponSword.h"
#include "Unit.h"


WeaponSword::~WeaponSword()
{
}

std::wstring WeaponSword::GetName()
{
	return L"Schwert";
}

bool WeaponSword::Intersects(UnitLocation& loc)
{
	auto b = this->GetBounds();
	return (b.left <= loc.x && b.top <= loc.y && b.bottom >= loc.y && b.right >= loc.x);
}

float WeaponSword::GetDamage()
{
	return 10.0f;
}

void WeaponSword::OnHit(Unit& unit)
{
	unit.GetStats().Substract(Stat::Health, this->GetDamage());
}

D2D1_RECT_F WeaponSword::GetBounds()
{
	auto wp_pos = mOwner->GetLocation();
	return D2D1::RectF(wp_pos.x - 10.0f, wp_pos.y - 5.0f, wp_pos.x, wp_pos.y);
}

ID2D1Bitmap* WeaponSword::GetGraphic()
{
	return D2Pool::GetGraphic(D2Graphic::SWORD);
}
