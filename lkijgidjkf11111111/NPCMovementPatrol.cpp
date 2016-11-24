#include "NPCMovementPatrol.h"



NPCMovementPatrol::NPCMovementPatrol()
	: mLastWaypoint(mWaypoints.end())
{
}


NPCMovementPatrol::~NPCMovementPatrol()
{
}

void NPCMovementPatrol::AddWaypoint(float x, float y)
{
	mWaypoints.push_back(UnitLocation(x, y));
	mLastWaypoint = mWaypoints.end();
}

void NPCMovementPatrol::RemoveWaypoint(int index)
{
	if (index < mWaypoints.size()) {
		mWaypoints.erase(mWaypoints.begin() + index);
		mLastWaypoint = mWaypoints.begin();
	}
}

UnitLocation NPCMovementPatrol::GetNextWaypoint()
{
	if (mWaypoints.size() > 0) {
		if (mLastWaypoint != mWaypoints.end()) {
			++mLastWaypoint;
		}

		if (mLastWaypoint == mWaypoints.end()) {
			mLastWaypoint = mWaypoints.begin();
		}

		return *mLastWaypoint;
	}
	else
		return UnitLocation::NONE;
}

bool NPCMovementPatrol::IsEmpty()
{
	return mWaypoints.empty();
}

void NPCMovementPatrol::Clear()
{
	mWaypoints.clear();
}
