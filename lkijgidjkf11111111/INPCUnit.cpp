#include "INPCUnit.h"



INPCUnit::INPCUnit()
	: mMovement(nullptr)
{
}


INPCUnit::~INPCUnit()
{
}

UnitLocation INPCUnit::GetLocation()
{
	if (mMovement) {
		auto loc = mMovement->GetLocation();
		if (!loc.IsNone())
			return loc;
	}

	return Unit::GetLocation();
}

void INPCUnit::OnUpdate(uint64_t diff_ms)
{
	/*if (anim_positionX.HasEnded() && mMovement) {
		auto wp = mMovement->GetNextWaypoint();
		if (!wp.IsNone()) {
			auto loc = this->GetLocation();
			auto diff = wp - loc;
			float dist = loc.DistanceTo(wp);
			TimeDiff duration = round(dist / this->GetSpeed() * 1000);
			anim_positionX.Reset(loc.x, wp.x, duration);
			anim_positionY.Reset(loc.y, wp.y, duration);
		}
	}*/
}

void INPCUnit::SetMovement(INPCMovement *mvmt)
{
	this->mMovement = mvmt;
}

void INPCUnit::Halt()
{
	if (mMovement)
		mMovement->Clear();
}

INPCMovement* INPCUnit::GetMovement()
{
	return mMovement;
}
