#include "ProgressBarLayer.h"

ProgressBarLayer::ProgressBarLayer(int width, int height)
	: Layer(width, height), nRedraw(1), mBackground(D2D1::ColorF(D2D1::ColorF::CornflowerBlue))
{

}

ProgressBarLayer::~ProgressBarLayer()
{
}

void ProgressBarLayer::OnUpdate()
{
	float val = this->GetOpacity() + 0.01f;
	//this->SetOpacity(val > 1.0f ? 0.0f : val);
}

bool ProgressBarLayer::OnDraw(ID2D1RenderTarget* target)
{
	target->Clear(mBackground);

#define TILE_COUNT 40
#define TILE_SIZE (float(this->GetWidth()) / float(TILE_COUNT))

	int num = nRedraw * 25;

	D2D1_RECT_F rect1 = D2D1::RectF(num % this->GetWidth(), 0);
	rect1.right = rect1.left + this->GetWidth() / 2;
	rect1.bottom = mHeight;

	D2D1_RECT_F rect2 = D2D1::RectF(rect1.left - this->GetWidth(), 0);
	rect2.right = rect2.left + this->GetWidth() / 2;
	rect2.bottom = mHeight;

//	D2D1_RECT_F rect = D2D1::RectF(rect.left + TILE_SIZE, rect.top, rect.right + TILE_SIZE, rect.bottom);

	target->FillRectangle(rect1, D2Pool::GetSolidColorBrush(D2D1::ColorF::White));
	target->FillRectangle(rect2, D2Pool::GetSolidColorBrush(D2D1::ColorF::White));

	++nRedraw;

	return false;
}

bool MUST_CALL ProgressBarLayer::OnMouseMove(int x, int y)
{

	return Layer::OnMouseMove(x, y);
}

bool MUST_CALL ProgressBarLayer::OnLMouseDown(int x, int y)
{
	nRedraw = x;
	return Layer::OnLMouseDown(x, y);
}
