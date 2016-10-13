#include "Layer.h"

#include <queue>

void Layer::SetTarget(ID2D1BitmapRenderTarget *target)
{
	mTarget = target;
	if (target) {
		D2D1_SIZE_U size = target->GetPixelSize();
		mWidth = size.width;
		mHeight = size.height;
	}
}

void Layer::DrawFromBackBuffer(ID2D1RenderTarget *target, D2D1_RECT_F& rect)
{
	target->DrawBitmap(this->GetBitmap(), rect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, rect);
}

Layer::Layer(int width, int height)
	: Drawable(D2Pool::CreateRenderTarget(width, height)), mWidth(width), mHeight(height), mOpacity(1.0f), mVisible(true), mAlphaBackground(false)
{

}

//Layer::Layer(ID2D1RenderTarget* target)
//	: Drawable(target), mOpacity(1.0f), mVisible(true), mAlphaBackground(false)
//{
//	this->SetTarget(target);
//}

Layer::~Layer()
{
}

void Layer::AddLayer(Layer *layer)
{
	layer->SetParent(this);
	mLayers.push_back(layer);
}

void Layer::SetPosition(int x, int y)
{
	mPosX = x;
	mPosY = y;

	this->InvalidateParent();
}

int Layer::GetPositionX()
{
	return mPosX;
}

int Layer::GetPositionY()
{
	return mPosY;
}

int Layer::GetWidth()
{
	return mWidth;
}

int Layer::GetHeight()
{
	return mHeight;
}

D2D1_RECT_F Layer::GetRectangle()
{
	return D2D1::RectF(0, 0, mWidth, mHeight);
}

D2D1_RECT_F Layer::GetBounds()
{
	float x = this->GetPositionX(), y = this->GetPositionY();
	return D2D1::RectF(x, y, x + this->GetWidth(), y + this->GetHeight());
}

Layer* Layer::GetParent()
{
	return mParent;
}

void Layer::SetParent(Layer* layer)
{
	mParent = layer;
}

void Layer::InvalidateParent()
{
	if (this->GetParent())
		this->GetParent()->Invalidate(INVALIDATION_MODE::INVALIDATION_ALL);
}

float Layer::GetOpacity()
{
	return mOpacity;
}

void Layer::SetOpacity(float opacity)
{
	mOpacity = opacity;
	this->InvalidateParent();
}

void Layer::SetVisible(bool value)
{
	mVisible = value;
	this->InvalidateParent();
}

bool Layer::IsVisible()
{
	return mVisible;
}

bool Layer::Intersects(int x, int y)
{
	return (mPosX <= x && (mPosX + mWidth) >= x)
		&& (mPosY <= y && (mPosY + mHeight) >= y);
}

bool Layer::IsTransparent()
{
	return mAlphaBackground || mOpacity < 1.0f;
}

void Layer::Update()
{
	Drawable::Update();

	for each(Layer *layer in mLayers) {
		layer->Update();
	}
}

void Layer::Draw(ID2D1RenderTarget* target)
{
	std::queue<Layer*> batch;

	this->StartDrawing(target);

	for each(Layer *layer in mLayers) {
		bool childWasValid = layer->IsValid();
		if (!layer->IsVisible())
			continue;
		layer->Draw(layer->GetTarget());
		if (!!layer->GetBitmap())
			batch.push(layer);
	}

	Drawable::Draw(target);

	while (!batch.empty()) {
		Layer *l = batch.front();
		//target->FillRectangle(l->GetBounds(), D2Pool::GetSolidColorBrush(D2D1::ColorF::Maroon));
		target->DrawBitmap(l->GetBitmap(), l->GetBounds(), l->GetOpacity(), D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, l->GetRectangle());
		batch.pop();
	}

	this->EndDrawing(target);
}

bool Layer::IsBitmap()
{
	return true;
}

bool MUST_CALL Layer::OnMouseMove(int x, int y)
{
	//for each (auto l in mLayers) {
	
	for (auto it = mLayers.rbegin(); it != mLayers.rend(); ++it) {
		Layer *l = *it;
		
		if (l->Intersects(x, y)) {
			l->OnMouseMove(x - l->GetPositionX(), y - l->GetPositionY());
			return false;
		}
	}
	return true;
}

bool MUST_CALL Layer::OnLMouseDown(int x, int y)
{
	for (auto it = mLayers.rbegin(); it != mLayers.rend(); ++it) {
		Layer *l = *it;
		
		if (l->Intersects(x, y)) {
			l->OnLMouseDown(x - l->GetPositionX(), y - l->GetPositionY());
			return false;
		}
	}
	return true;
}

bool MUST_CALL Layer::OnRMouseDown(int x, int y)
{
	for (auto it = mLayers.rbegin(); it != mLayers.rend(); ++it) {
		Layer *l = *it;

		if (l->Intersects(x, y)) {
			l->OnRMouseDown(x - l->GetPositionX(), y - l->GetPositionY());
			return false;
		}
	}
	return true;
}

bool MUST_CALL Layer::OnLMouseUp(int x, int y)
{
	for (auto it = mLayers.rbegin(); it != mLayers.rend(); ++it) {
		Layer *l = *it;

		if (l->Intersects(x, y)) {
			l->OnLMouseUp(x - l->GetPositionX(), y - l->GetPositionY());
			return false;
		}
	}
	return true;
}

bool MUST_CALL Layer::OnRMouseUp(int x, int y)
{
	for (auto it = mLayers.rbegin(); it != mLayers.rend(); ++it) {
		Layer *l = *it;

		if (l->Intersects(x, y)) {
			l->OnRMouseUp(x - l->GetPositionX(), y - l->GetPositionY());
			return false;
		}
	}
	return true;
}
