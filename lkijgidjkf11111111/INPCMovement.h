#pragma once
#include "Unit.h"

class INPCMovement abstract
{
public:
	INPCMovement();
	virtual ~INPCMovement();

	virtual UnitLocation GetNextWaypoint() = 0;
	virtual bool IsEmpty() = 0;
	virtual void Clear() = 0;
};

