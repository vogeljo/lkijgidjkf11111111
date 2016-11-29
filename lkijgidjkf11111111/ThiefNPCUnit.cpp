#include "ThiefNPCUnit.h"



ThiefNPCUnit::ThiefNPCUnit()
{
	this->SetName(L"Thief");
	this->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
	this->GetStats().Set(Stat::Health, 100);

	auto mvmt = new NPCMovementPatrol(this);

	this->SetMovement(mvmt);
	this->SetSpeed(10.0f);
}


ThiefNPCUnit::~ThiefNPCUnit()
{
}

NPCMovementPatrol * ThiefNPCUnit::GetPatrol()
{
	return dynamic_cast<NPCMovementPatrol*>(mMovement);
}
