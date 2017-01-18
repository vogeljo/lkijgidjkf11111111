#pragma once
#include "Layer.h"
#include "HouseInfo.h"

class HouseLayer :
	public Layer
{
private:
	HouseInfo *mHouse;
public:
	HouseLayer(HouseInfo *house, Game& game, int width, int height);
	virtual ~HouseLayer();

	virtual void OnUpdate() override;
	virtual bool OnDraw(ID2D1RenderTarget* target) override;

	void SetHouse(HouseInfo *house);

};

