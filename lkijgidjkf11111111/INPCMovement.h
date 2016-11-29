#pragma once
#include "Unit.h"

class INPCMovement abstract
{
public:
	INPCMovement();
	virtual ~INPCMovement();
	
	virtual bool IsEmpty() = 0;
	virtual void Clear() = 0;
	virtual int GetCount() = 0;

	virtual UnitLocation GetLocation() = 0;

	virtual float GetSpeed() = 0;
};

