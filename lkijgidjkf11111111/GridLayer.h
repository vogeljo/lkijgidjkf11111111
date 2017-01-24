#pragma once
#include "Layer.h"

class GridLayer :
	public Layer
{
private:
	int mRows;
	int mColumns;
	bool mDrawGrid;

	float GetColumnWidth();
	float GetRowHeight();
public:
	GridLayer(Game& game, int width, int height, int rows, int columns);
	virtual ~GridLayer();

	virtual void OnUpdate() override;
	virtual bool OnDraw(ID2D1RenderTarget* target) override;

	void SetDrawGrid(bool value);

	virtual void OnLayerAdded(Layer *layer) override;

};

