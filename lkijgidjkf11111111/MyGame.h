#pragma once
#include "Game.h"
#include "Player.h"
#include "ProgressBarLayer.h"
#include "CashDisplayLayer.h"
#include "MapLayer.h"

#include "MapData.h"
#include "MyGameItems.h"
#include "InventoryLayer.h"

class MyGame :
	public Game
{
private:
	int nUpdate;
	Player uPlayer;

	ProgressBarLayer *cl;
	CashDisplayLayer *l_cash;
	MapLayer *l_map;
	InventoryLayer *l_inventory;

	MapData *map_ptr;
public:
	MyGame(int width, int height);
	virtual ~MyGame();
	using Game::Game;

	void OpenInventory();

	virtual void OnUpdate() override;
	virtual bool OnDraw(ID2D1RenderTarget* target) override;


	virtual void Initialize() override;
	virtual bool MUST_CALL OnMouseMove(int x, int y) override;

	virtual bool MUST_CALL OnLMouseDown(int x, int y) override;


	virtual void OnKeyDown(int key) override;

};

