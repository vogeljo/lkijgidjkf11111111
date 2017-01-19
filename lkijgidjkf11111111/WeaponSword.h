#pragma once
#include "IWeapon.h"
class WeaponSword :
	public IWeapon
{
public:
	using IWeapon::IWeapon;
	virtual ~WeaponSword();

	virtual std::wstring GetName() override;
	virtual bool Intersects(UnitLocation& loc) override;
	virtual float GetDamage() override;
	virtual void OnHit(Unit& unit) override;
	virtual D2D1_RECT_F GetBounds() override;
	virtual ID2D1Bitmap* GetGraphic() override;

};

