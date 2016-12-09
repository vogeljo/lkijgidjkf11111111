#include "Layer.h"

#include "Game.h"

void Layer::SetTarget(ID2D1BitmapRenderTarget *target)
{
	mTarget = target;
	if (target) {
		D2D1_SIZE_U size = target->GetPixelSize();
		mWidth = size.width;
		mHeight = size.height;
	}
}

Layer::Layer(Game& game, int width, int height)
	: Drawable(D2Pool::CreateRenderTarget(width, height)), mGame(game), mWidth(width), mHeight(height), mVisible(true), mAlphaBackground(false)
{
	dynOpacity.Reset(1.0f, 1.0f, 0);
}

Layer::~Layer()
{
}

Game& Layer::GetGame()
{
	return mGame;
}

void Layer::AddLayer(Layer *layer)
{
	Layer *lower = nullptr;
	if (mLayers.size())
		lower = mLayers[mLayers.size() - 1];

	this->AddLayer(layer, lower);
}

void Layer::AddLayer(Layer *layer, Layer *lower)
{
	layer->SetParent(this);

	auto it = mLayers.begin() + mLayers.size();

	if (lower) {
		for (auto i = mLayers.begin(); i != mLayers.end(); ++i) {
			if (*i == lower) {
				it = i + 1;
				continue;
			}
		}
	}

	mLayers.insert(it, layer);
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

D2D1_RECT_F Layer::GetContentRectangle()
{
	return D2D1::RectF(mPadding.left, mPadding.top, mWidth - mPadding.right, mHeight - mPadding.bottom);
}

D2D1_RECT_F Layer::GetBoundingRectangle()
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
		this->GetParent()->Invalidate();
}

float Layer::GetOpacity()
{
	auto val = dynOpacity.Get();
	return val;
}

void Layer::SetOpacity(float opacity)
{
	dynOpacity.Reset(opacity, opacity, 0);
	this->InvalidateParent();
}

void Layer::SetVisible(bool value)
{
	mVisible = value;
	this->InvalidateParent();
}

void Layer::SetPadding(float left, float top, float right, float bottom)
{
	mPadding.left = left;
	mPadding.top = top;
	mPadding.right = right;
	mPadding.bottom = bottom;

	this->Invalidate();
}

void Layer::SetPadding(float padding)
{
	this->SetPadding(padding, padding, padding, padding);
}

bool Layer::IsVisible()
{
	return mVisible;
}

bool Layer::IsOpaque()
{
	return this->GetOpacity() >= 1.0f;
}

bool Layer::Intersects(int x, int y)
{
	return (mPosX <= x && (mPosX + mWidth) >= x)
		&& (mPosY <= y && (mPosY + mHeight) >= y);
}

Layer* Layer::GetLayerAt(int x, int y)
{
	for (auto it = mLayers.rbegin(); it != mLayers.rend(); ++it) {
		Layer *l = *it;
		if (l->IsVisible() && l->TestMouseHit(x, y))
			return l;
	}
	return this;
}

Layer* Layer::FindLayerAt(int x, int y)
{
	auto child = this->GetLayerAt(x, y);
	if (child == this)
		return child;
	else
		return child->FindLayerAt(x - child->GetPositionX(), y - child->GetPositionY());
}

bool Layer::TestMouseHit(int x, int y)
{
	return this->Intersects(x, y);
}

bool Layer::IsTransparent()
{
	return mAlphaBackground || dynOpacity.Get() < 1.0f;
}

bool Layer::HasFocus()
{
	return mGame.GetFocused() == this;
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
	this->StartDrawing(target);

	for each(Layer *layer in mLayers) {
		if (!layer->IsVisible()  || layer->IsValid())
			continue;
		layer->Draw(layer->GetTarget());
	}

	Drawable::Draw(target);

	for each(Layer *layer in mLayers) {
		if (layer->IsVisible())
			layer->Print(target);
	}

	this->EndDrawing(target);
}

void Layer::Print(ID2D1RenderTarget *target)
{
	auto p = this->GetParent();

	float opacity = this->GetOpacity();
	if (p)
		opacity *= p->GetOpacity();

	target->DrawBitmap(this->GetBitmap(), this->GetBounds(), opacity);

	for each(auto l in mLayers)
		l->Print(target);
}

bool Layer::IsBitmap()
{
	return true;
}

bool MUST_CALL Layer::OnMouseMove(int x, int y)
{	
	Layer *l = this->GetLayerAt(x, y);
	if (l != this) {
		l->OnMouseMove(x - l->GetPositionX(), y - l->GetPositionY());
		return false;
	}
	return true;
}

bool MUST_CALL Layer::OnLMouseDown(int x, int y)
{
	Layer *l = this->GetLayerAt(x, y);
	if (l != this) {
		l->OnLMouseDown(x - l->GetPositionX(), y - l->GetPositionY());
		return false;
	}
	else
		mGame.SetFocus(this);
	return true;
}

bool MUST_CALL Layer::OnRMouseDown(int x, int y)
{
	Layer *l = this->GetLayerAt(x, y);
	if (l != this) {
		l->OnRMouseDown(x - l->GetPositionX(), y - l->GetPositionY());
		return false;
	}
	else
		mGame.SetFocus(this);
	return true;
}

bool MUST_CALL Layer::OnLMouseUp(int x, int y)
{
	Layer *l = this->GetLayerAt(x, y);
	if (l != this) {
		l->OnLMouseUp(x - l->GetPositionX(), y - l->GetPositionY());
		return false;
	}
	return true;
}

bool MUST_CALL Layer::OnRMouseUp(int x, int y)
{
	Layer *l = this->GetLayerAt(x, y);
	if (l != this) {
		l->OnRMouseUp(x - l->GetPositionX(), y - l->GetPositionY());
		return false;
	}
	return true;
}

bool MUST_CALL Layer::OnVMouseScroll(int x, int y, float delta)
{
	Layer *l = this->GetLayerAt(x, y);
	if (l != this) {
		l->OnVMouseScroll(x, y, delta);
		return false;
	}
	return true;
}

bool MUST_CALL Layer::OnHMouseScroll(int x, int y, float delta)
{
	Layer *l = this->GetLayerAt(x, y);
	if (l != this) {
		l->OnHMouseScroll(x, y, delta);
		return false;
	}
	return true;
}

void Layer::OnKeyDown(int key)
{

}

void MUST_CALL Layer::OnKeyChar(wchar_t c)
{

}

void MUST_CALL Layer::OnMouseEnter()
{

}

void MUST_CALL Layer::OnMouseLeave()
{

}

void Layer::FadeIn(DWORD duration_ms /*= 100*/)
{
	if (!this->IsVisible() && dynOpacity.HasEnded()) {
		this->SetVisible(true);
		dynOpacity.Reset(0.0f, 1.0f, duration_ms);
		this->Invalidate();
	}
}

void Layer::FadeOut(DWORD duration_ms /*= 100*/)
{
	if (this->IsVisible() && dynOpacity.HasEnded()) {
		dynOpacity.Reset(this->GetOpacity(), 0.0f, duration_ms, [&]() {
			this->SetVisible(false);
		});
	}
}
