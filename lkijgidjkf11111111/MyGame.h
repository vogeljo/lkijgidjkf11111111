#pragma once
#include "Game.h"

#include "ProgressBarLayer.h"
#include "CashDisplayLayer.h"

class MyGame :
	public CGame
{
private:
	int nUpdate;
	ProgressBarLayer *cl;
	CashDisplayLayer *l_cash;
public:
	MyGame(int width, int height);
	virtual ~MyGame();

	virtual void OnUpdate() override;
	virtual bool OnDraw(ID2D1RenderTarget* target) override;


	virtual void Initialize() override;
	virtual bool MUST_CALL OnMouseMove(int x, int y) override;

	virtual bool MUST_CALL OnLMouseDown(int x, int y) override;

};

