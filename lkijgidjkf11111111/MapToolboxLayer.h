#pragma once
#include "Layer.h"
class MapToolboxLayer :
	public Layer
{
public:
	MapToolboxLayer(Game& game, int width, int height);
	virtual ~MapToolboxLayer();

	// Geerbt über Layer
	virtual void OnUpdate() override;
	virtual bool OnDraw(ID2D1RenderTarget * target) override;
};

