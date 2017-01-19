#pragma once
#include "IWeapon.h"
class WeaponKnife :
	public IWeapon
{
public:
	using IWeapon::IWeapon;
	virtual ~WeaponKnife();

	virtual D2D1_RECT_F GetBounds() override;
	virtual float GetDamage() override;
	virtual ID2D1Bitmap* GetGraphic() override;
	virtual std::wstring GetName() override;
	virtual bool Intersects(UnitLocation& loc) override;
	virtual void OnHit(Unit& unit) override;

};

