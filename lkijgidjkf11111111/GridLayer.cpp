#include "GridLayer.h"



float GridLayer::GetColumnWidth()
{
	return (float)this->GetWidth() / (float)mColumns;
}

float GridLayer::GetRowHeight()
{
	return (float)this->GetHeight() / (float)mRows;
}

GridLayer::GridLayer(Game& game, int width, int height, int rows, int columns)
	: Layer(game, width, height), mRows(rows), mColumns(columns), mDrawGrid(false)
{

}


GridLayer::~GridLayer()
{
}

void GridLayer::OnUpdate()
{

}


bool GridLayer::OnDraw(ID2D1RenderTarget* target)
{
	target->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
	target->Clear(D2D1::ColorF(D2D1::ColorF::White, 0.0f));

	float cw = this->GetColumnWidth();
	float rh = this->GetRowHeight();

	if (mDrawGrid) {

		for (float x = cw; x < this->GetWidth(); x += cw) {
			target->DrawLine(D2D1::Point2F(x, 0.0f), D2D1::Point2F(x, this->GetHeight()), D2Pool::GetSolidColorBrush(D2D1::ColorF::LightGray));
		}

		for (float y = rh; y < this->GetHeight(); y += rh) {
			target->DrawLine(D2D1::Point2F(0.0f, y), D2D1::Point2F(this->GetWidth(), y), D2Pool::GetSolidColorBrush(D2D1::ColorF::LightGray));
		}
	}
	
	return true;
}

void GridLayer::SetDrawGrid(bool value)
{
	mDrawGrid = value;
	this->Invalidate();
}

void GridLayer::OnLayerAdded(Layer *layer)
{
	int index = mLayers.size() - 1;
	int column = index % mColumns;
	int row = index / mColumns;

	float cw = this->GetColumnWidth();
	float rh = this->GetRowHeight();

	auto rect = D2D1::RectF(column * cw, row * rh, (column + 1) * cw, (row + 1) * rh);
	
	layer->SetPosition(rect.left, rect.top);
}
