#pragma once
#include "Unit.h"
#include "INPCMovement.h"

#include "TweenDynamic.h"

class INPCUnit abstract :
	public Unit
{
public:
	INPCUnit();
	virtual ~INPCUnit();
	

	virtual UnitLocation GetLocation() override;
	INPCMovement* GetMovement();

	// Stops the npc unit's movement
	void Halt();
protected:
	virtual void OnUpdate(uint64_t diff_ms) override;

	LinearTweenDynamic<float> anim_positionX;
	LinearTweenDynamic<float> anim_positionY;

	INPCMovement *mMovement;

	void SetMovement(INPCMovement *mvmt);
};

