#pragma once
#include "Unit.h"

class INPCMovement abstract
{
public:
	INPCMovement();
	virtual ~INPCMovement();

	virtual UnitLocation GetNextWaypoint() = 0;
};

