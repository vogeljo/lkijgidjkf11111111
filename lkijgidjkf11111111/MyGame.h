#pragma once
#include "Game.h"
#include "Unit.h"
#include "ProgressBarLayer.h"
#include "CashDisplayLayer.h"
#include "MapLayer.h"

class MyGame :
	public CGame
{
private:
	int nUpdate;
	Unit uPlayer;

	ProgressBarLayer *cl;
	CashDisplayLayer *l_cash;
	MapLayer *l_map;
public:
	MyGame(int width, int height);
	virtual ~MyGame();

	virtual void OnUpdate() override;
	virtual bool OnDraw(ID2D1RenderTarget* target) override;


	virtual void Initialize() override;
	virtual bool MUST_CALL OnMouseMove(int x, int y) override;

	virtual bool MUST_CALL OnLMouseDown(int x, int y) override;

};

