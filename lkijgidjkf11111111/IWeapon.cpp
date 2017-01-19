#include "IWeapon.h"

#include "Unit.h"


IWeapon::IWeapon(Unit *owner)
	: mOwner(owner)
{
}


IWeapon::~IWeapon()
{
}

void IWeapon::SetOwner(Unit *unit)
{
	mOwner = unit;
}

Unit* IWeapon::GetOwner()
{
	return mOwner;
}

float IWeapon::GetAngleRad()
{
	return mAngle;
}

void IWeapon::SetAngleRad(float value)
{
	mAngle = value;
}
