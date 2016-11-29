#pragma once
#include "INPCMovement.h"
#include <vector>

#include "TweenDynamic.h"

class NPCMovementPatrol :
	public INPCMovement
{
private:
	std::vector<UnitLocation> mWaypoints;
	std::vector<UnitLocation>::iterator mLastWaypoint;

	LinearTweenDynamic<float> mTweenX, mTweenY;

	Unit *mUnit;
public:
	NPCMovementPatrol(Unit *unit);
	virtual ~NPCMovementPatrol();

	void AddWaypoint(float x, float y);
	void RemoveWaypoint(int index);

	virtual bool IsEmpty() override;
	virtual void Clear() override;
	virtual int GetCount() override;

	virtual float GetSpeed() override;

	// Geerbt über INPCMovement
	virtual UnitLocation GetLocation() override;
};

