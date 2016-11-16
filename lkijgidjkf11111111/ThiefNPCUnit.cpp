#include "ThiefNPCUnit.h"



ThiefNPCUnit::ThiefNPCUnit()
{
	this->SetName(L"Thief");
	this->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
	this->GetStats().Set(Stat::Health, 100);

	auto mvmt = new NPCMovementPatrol();
	mvmt->AddWaypoint(10.0f, 10.0f);
	mvmt->AddWaypoint(30.0f, 10.0f);
	mvmt->AddWaypoint(25.0f, 30.0f);
	mvmt->AddWaypoint(05.0f, 30.0f);

	this->SetMovement(mvmt);
}


ThiefNPCUnit::~ThiefNPCUnit()
{
}