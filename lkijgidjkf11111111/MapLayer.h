#pragma once
#include "Layer.h"

#include "MapData.h"

class MapLayer :
	public Layer
{
protected:
	int mTileSize;
	int mMouseTileX;
	int mMouseTileY;
	bool mMouseHover;
	int mOffsetX;
	int mOffsetY;
	int mDragX, mDragY;
	int mDragOffsetX, mDragOffsetY;
	bool mDragging;

	MapData mMapData;

	D2D1_RECT_F GetRectForCell(int cellX, int cellY);
public:
	MapLayer(int width, int height);
	virtual ~MapLayer();

	virtual void OnUpdate() override;
	virtual bool OnDraw(ID2D1RenderTarget* target) override;

	void SetTileSize(int value);
	
	virtual bool MUST_CALL OnMouseMove(int x, int y) override;
	virtual bool MUST_CALL OnRMouseDown(int x, int y) override;
	virtual bool MUST_CALL OnRMouseUp(int x, int y) override;

};

