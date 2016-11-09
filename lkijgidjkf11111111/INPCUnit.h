#pragma once
#include "Unit.h"
#include "INPCMovement.h"
#include "LinearTimedAnimation.h"

class INPCUnit abstract :
	public Unit
{
public:
	INPCUnit();
	virtual ~INPCUnit();
	

	virtual UnitLocation GetLocation() override;
	INPCMovement* GetMovement();

protected:
	virtual void OnUpdate(uint64_t diff_ms) override;

	LinearTimedAnimation anim_positionX;
	LinearTimedAnimation anim_positionY;

	INPCMovement *mMovement;
	bool mAnimationsEnded;

	void SetMovement(INPCMovement *mvmt);
};

