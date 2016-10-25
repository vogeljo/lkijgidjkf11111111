#pragma once
#include "Layer.h"

class ProgressBarLayer :
	public Layer
{
protected:
	int nRedraw;
	D2D1_COLOR_F mBackground;
public:
	ProgressBarLayer(Game& game, int width, int height);
	virtual ~ProgressBarLayer();

	virtual void OnUpdate() override;


	virtual bool OnDraw(ID2D1RenderTarget* target) override;


	virtual bool MUST_CALL OnMouseMove(int x, int y) override;
	virtual bool MUST_CALL OnLMouseDown(int x, int y) override;

};

