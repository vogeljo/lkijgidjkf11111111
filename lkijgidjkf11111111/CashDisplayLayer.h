#pragma once
#include "Layer.h"
class CashDisplayLayer :
	public Layer
{
private:
	int mValue;
public:
	CashDisplayLayer(int value, int width, int height);
	virtual ~CashDisplayLayer();

	void SetValue(int value);

	virtual void OnUpdate() override;


	virtual bool OnDraw(ID2D1RenderTarget* target) override;

};

