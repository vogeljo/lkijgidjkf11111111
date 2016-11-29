#include "NPCMovementPatrol.h"

NPCMovementPatrol::NPCMovementPatrol(Unit * unit)
	: mUnit(unit), mLastWaypoint(mWaypoints.end())
{
}

NPCMovementPatrol::~NPCMovementPatrol()
{
}

void NPCMovementPatrol::AddWaypoint(float x, float y)
{
	auto c_index = mLastWaypoint - mWaypoints.begin();
	mWaypoints.push_back(UnitLocation(x, y));
	mLastWaypoint = mWaypoints.begin() + c_index;
}

void NPCMovementPatrol::RemoveWaypoint(int index)
{
	if (index < mWaypoints.size()) {
		mWaypoints.erase(mWaypoints.begin() + index);
		mLastWaypoint = mWaypoints.begin();
	}
}

bool NPCMovementPatrol::IsEmpty()
{
	return mWaypoints.empty();
}

void NPCMovementPatrol::Clear()
{
	mWaypoints.clear();
}

int NPCMovementPatrol::GetCount()
{
	return mWaypoints.size();
}

float NPCMovementPatrol::GetSpeed()
{
	return mUnit->GetSpeed();
}

UnitLocation NPCMovementPatrol::GetLocation()
{
	if (mWaypoints.empty())
		return UnitLocation::NONE;

	auto last = mLastWaypoint;
	auto last_val = *last;

	if (mWaypoints.size() == 1) {
		return last_val;
	}

	if (mTweenX.HasEnded()) {
		auto next = last + 1;
		if (next == mWaypoints.end()) {
			next = mWaypoints.begin();
		}

		auto next_val = *next;

		auto distance = last_val.DistanceTo(next_val);
		auto duration = distance / this->GetSpeed() * 1000;

		mTweenX.Reset(last_val.x, next_val.x, duration);
		mTweenY.Reset(last_val.y, next_val.y, duration);

		printf("Tweens reset.\n");
		printf("X: %f -> %f\n", last_val.x, next_val.x);
		printf("Y: %f -> %f\n", last_val.y, next_val.y);

		mLastWaypoint = next;
	}

	return UnitLocation(mTweenX.Get(), mTweenY.Get());
}
