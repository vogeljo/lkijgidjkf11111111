#include "Layer.h"


void Layer::SetTarget(ID2D1RenderTarget *target)
{
	mTarget = target;
	if (target) {
		D2D1_SIZE_U size = target->GetPixelSize();
		mWidth = size.width;
		mHeight = size.height;
	}
}

Layer::Layer(int width, int height)
	: Drawable(D2Pool::CreateRenderTarget(width, height)), mWidth(width), mHeight(height), mOpacity(1.0f), mVisible(true)
{

}

Layer::Layer(ID2D1RenderTarget* target)
	: Drawable(target), mOpacity(1.0f), mVisible(true)
{
	this->SetTarget(target);
}

Layer::~Layer()
{
}

void Layer::AddLayer(Layer *layer)
{
	layer->SetParent(this);
	mLayers.insert(layer);
}

void Layer::SetPosition(int x, int y)
{
	mPosX = x;
	mPosY = y;

	if (this->GetParent())
		this->GetParent()->Invalidate(INVALIDATION_MODE::INVALIDATION_ALL);
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

void Layer::Update()
{
	Drawable::Update();

	for each(Layer *layer in mLayers) {
		layer->Update();
	}
}

void Layer::Draw(ID2D1RenderTarget* target)
{
	//INVALIDATION_MODE invMode = this->GetInvalidationMode();
	this->StartDrawing(target);
	Drawable::Draw(target);

	if (mLayers.size() != 0)
	{
		for each(Layer *layer in mLayers) {
			if (!layer->IsVisible())
				continue;
			layer->Draw(layer->GetTarget());
			ID2D1Bitmap *bmp = layer->GetBitmap();
			if (bmp)
				target->DrawBitmap(bmp, layer->GetBounds(), layer->GetOpacity(), D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, layer->GetRectangle());
		}
	}
	this->EndDrawing(target);
}

bool Layer::IsBitmap()
{
	return true;
}

bool MUST_CALL Layer::OnMouseMove(int x, int y)
{
	for each (auto l in mLayers) {
		if (l->Intersects(x, y)) {
			l->OnMouseMove(x - l->GetPositionX(), y - l->GetPositionY());
			return false;
		}
	}
	return true;
}

bool MUST_CALL Layer::OnLMouseDown(int x, int y)
{
	for each (auto l in mLayers) {
		if (l->Intersects(x, y)) {
			l->OnLMouseDown(x - l->GetPositionX(), y - l->GetPositionY());
			return false;
		}
	}
	return true;
}
