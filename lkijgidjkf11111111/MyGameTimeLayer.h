#pragma once
#include "Layer.h"
#include "MyGameTime.h"
#include "D2Pool.h"

class MyGameTimeLayer :
	public Layer
{
private:
	MyGameTime& mTime;
public:
	MyGameTimeLayer(Game& game, int width, int height, MyGameTime& time);
	virtual ~MyGameTimeLayer();

	virtual void OnUpdate() override;
	virtual bool OnDraw(ID2D1RenderTarget* target) override;

};

