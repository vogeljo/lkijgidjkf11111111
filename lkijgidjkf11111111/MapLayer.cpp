#include "MapLayer.h"

#include <cmath>

D2D1_RECT_F MapLayer::GetRectForCell(int cellX, int cellY)
{
	float x = cellX * mTileSize - mOffsetX;
	float y = cellY * mTileSize - mOffsetY;
	return D2D1::RectF(x, y, x + mTileSize - 1, y + mTileSize - 1);
}

MapLayer::MapLayer(int width, int height)
	: Layer(width, height), mMouseTileX(-1), mMouseTileY(-1), mOffsetX(4), mOffsetY(4), mDragging(false), mMouseHover(false)
{
	this->SetTileSize(32);
}

MapLayer::~MapLayer()
{
}

void MapLayer::OnUpdate()
{
	mOffsetX = max(0, mOffsetX);
	mOffsetY = max(0, mOffsetY);
}

bool MapLayer::OnDraw(ID2D1RenderTarget* target)
{
	target->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
	target->Clear(D2D1::ColorF(D2D1::ColorF::DarkGreen));

	int gridOffsetX = mOffsetX % mTileSize;
	int gridOffsetY = mOffsetY % mTileSize;

	int tileSize = mTileSize;
	for (int x = -gridOffsetX; x <= mWidth; x += tileSize) {
		target->DrawLine(D2D1::Point2F(x, 0), D2D1::Point2F(x, mHeight), D2Pool::GetSolidColorBrush(D2D1::ColorF::Black, 0.2f));
	}
	for (int y = -gridOffsetY; y <= mHeight; y += tileSize) {
		target->DrawLine(D2D1::Point2F(0, y), D2D1::Point2F(mWidth, y), D2Pool::GetSolidColorBrush(D2D1::ColorF::Black, 0.2f));
	}

	if (mMouseHover) {
		target->FillRectangle(this->GetRectForCell(mMouseTileX, mMouseTileY), D2Pool::GetSolidColorBrush(D2D1::ColorF::Black, 0.2f));
		D2Pool::PrintText(D2Pool::FormatString(L"(%n; %n)", mMouseTileX, mMouseTileY), target, D2Pool::GetFormat(D2PoolFont::NORMAL), this->GetRectangle(), D2Pool::GetSolidColorBrush(D2D1::ColorF::Red));
	}
	
	return true;
}

void MapLayer::SetTileSize(int value)
{
	mTileSize = max(5, value);
	this->Invalidate(INVALIDATION_ALL);
}

bool MUST_CALL MapLayer::OnMouseMove(int x, int y)
{
	if (mDragging) {
		mOffsetX = mDragOffsetX + (mDragX - x);
		mOffsetY = mDragOffsetY + (mDragY - y);

		mDragX = x;
		mDragY = y;
		mDragOffsetX = mOffsetX;
		mDragOffsetY = mOffsetY;

		mMouseHover = false;
		this->Invalidate(INVALIDATION_ALL);
	}

	if (Layer::OnMouseMove(x, y)) {
		mMouseHover = true;

		if (!mDragging) {
			mMouseTileX = floor((float)(x + mOffsetX) / (float)mTileSize);
			mMouseTileY = floor((float)(y + mOffsetY) / (float)mTileSize);
		}

		this->Invalidate(INVALIDATION_ALL);
		return true;
	}
	return false;
}

bool MUST_CALL MapLayer::OnRMouseDown(int x, int y)
{
	if (Layer::OnMouseMove(x, y)) {
		mDragX = x;
		mDragY = y;
		mDragOffsetX = mOffsetX;
		mDragOffsetY = mOffsetY;
		mDragging = true;
		return true;
	}
	return false;
}

bool MUST_CALL MapLayer::OnRMouseUp(int x, int y)
{
	mDragging = false;
	return true;
}
