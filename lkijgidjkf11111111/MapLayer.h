#pragma once
#include "Layer.h"

#include "InfoLayer.h"
#include "MapData.h"
#include "DragController.h"
#include "Player.h"
#include "Util.h"
#include "MyGameItems.h"

class MyGame;

class MapLayer :
	public Layer
{
protected:
	int mTileSize;
	int mMouseTileX;
	int mMouseTileY;
	bool mMouseHover;

	DragController mDragScroll;
	InfoLayer *l_info;
	MapData& mMapData;

	Player *mPlayer;

	int nUpdate;

	D2D1_RECT_F GetRectForCell(int cellX, int cellY);
	int GetTileForXY(int x);
public:
	MapLayer(Game& game, int width, int height, MapData& mapData);
	virtual ~MapLayer();

	virtual void Initialize() override;

	virtual void OnUpdate() override;
	virtual bool OnDraw(ID2D1RenderTarget* target) override;

	//void DrawLabelForUnit(ID2D1RenderTarget *target, Unit *unit, std::wstring label);
	void DrawUnit(ID2D1RenderTarget *target, Unit *unit);

	void SetTileSize(int value);
	void SetPlayer(Player *player);
	void CenterUnit(Unit *unit);
	
	virtual bool MUST_CALL OnMouseMove(int x, int y) override;
	virtual bool MUST_CALL OnRMouseDown(int x, int y) override;
	virtual bool MUST_CALL OnRMouseUp(int x, int y) override;
	virtual bool MUST_CALL OnLMouseDown(int x, int y) override;
	
	virtual void OnKeyDown(int key) override;
	virtual void MUST_CALL OnMouseLeave() override;

};

