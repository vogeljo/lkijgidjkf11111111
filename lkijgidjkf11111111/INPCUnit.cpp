#include "INPCUnit.h"



INPCUnit::INPCUnit()
	: mMovement(nullptr), mAnimationsEnded(true)
{
}


INPCUnit::~INPCUnit()
{
}

UnitLocation INPCUnit::GetLocation()
{
	if (!mAnimationsEnded) {
		return UnitLocation(anim_positionX.GetValue(), anim_positionY.GetValue());
	}
	else
		return Unit::GetLocation();
}

void INPCUnit::OnUpdate(uint64_t diff_ms)
{
	if (anim_positionX.IsRelaxing() && anim_positionY.IsRelaxing()) {
		if (!mAnimationsEnded) {
			this->SetLocation(anim_positionX.GetValue(), anim_positionY.GetValue());
			mAnimationsEnded = true;
		}
		if (mMovement) {
			auto wp = mMovement->GetNextWaypoint();
			if (!wp.IsNone()) {
				auto loc = this->GetLocation();
				auto diff = wp - loc;
				float dist = loc.DistanceTo(wp);
				anim_positionX.Start(loc.x, wp.x, round(dist / this->GetSpeed() * 1000));
				anim_positionY.Start(loc.y, wp.y, round(dist / this->GetSpeed() * 1000));
				mAnimationsEnded = false;
			}
		}
	}
}

void INPCUnit::SetMovement(INPCMovement *mvmt)
{
	this->mMovement = mvmt;
}

INPCMovement* INPCUnit::GetMovement()
{
	return mMovement;
}
