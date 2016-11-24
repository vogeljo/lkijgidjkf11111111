#pragma once
#include "INPCMovement.h"
#include <vector>

class NPCMovementPatrol :
	public INPCMovement
{
private:
	std::vector<UnitLocation> mWaypoints;
	std::vector<UnitLocation>::iterator mLastWaypoint;
public:
	NPCMovementPatrol();
	virtual ~NPCMovementPatrol();

	void AddWaypoint(float x, float y);
	void RemoveWaypoint(int index);

	virtual UnitLocation GetNextWaypoint() override;
	virtual bool IsEmpty() override;
	virtual void Clear() override;
};

