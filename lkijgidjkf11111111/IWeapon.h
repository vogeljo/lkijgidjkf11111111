#pragma once
#include <string>
#include "native.h"

class Unit;
struct UnitLocation;

class IWeapon abstract
{
protected:
	Unit *mOwner;
	float mAngle;
public:
	IWeapon(Unit *owner);
	virtual ~IWeapon();

	virtual std::wstring GetName() = 0;
	virtual bool Intersects(UnitLocation& loc) = 0;
	virtual float GetDamage() = 0;
	virtual void OnHit(Unit& unit) = 0;
	virtual D2D1_RECT_F GetBounds() = 0;
	virtual ID2D1Bitmap* GetGraphic() = 0;

	void SetOwner(Unit *unit);
	Unit* GetOwner();

	float GetAngleRad();
	void SetAngleRad(float value);
};

