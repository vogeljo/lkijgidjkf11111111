#include "Drawable.h"

void Drawable::StartDrawing(ID2D1RenderTarget *target)
{
	if (!mIsDrawing) {
		target->BeginDraw();
	}
	++mIsDrawing;
}

void Drawable::EndDrawing(ID2D1RenderTarget *target)
{
	--mIsDrawing;
	if (!mIsDrawing) {
		target->EndDraw();
	}
}

Drawable::Drawable(ID2D1BitmapRenderTarget* target)
	: mTarget(target), mInitalized(false), mInvalidationMode(INVALIDATION_DEFAULT)
{

}

Drawable::~Drawable()
{
}

bool Drawable::IsBitmap()
{
	return false;
}

void Drawable::Initialize()
{

}

ID2D1Bitmap* Drawable::GetBitmap()
{
	ID2D1Bitmap *bmp = nullptr;
	if (this->IsBitmap()) {
		ID2D1BitmapRenderTarget *ptr = static_cast<ID2D1BitmapRenderTarget*>(this->GetTarget());
		if (ptr)
			ptr->GetBitmap(&bmp);
	}
	return bmp;
}

ID2D1BitmapRenderTarget* Drawable::GetTarget()
{
	return mTarget;
}

void Drawable::Update()
{
	if (!mInitalized) {
		this->Initialize();
		mInitalized = true;
	}
	this->OnUpdate();
}

void Drawable::Draw(ID2D1RenderTarget* target)
{
	return this->Draw(target, false);
}

void Drawable::Draw(ID2D1RenderTarget* target, bool force)
{
	if (force || !this->IsValid()) {
		this->StartDrawing(target);
		if (this->OnDraw(target)) {
			this->Invalidate(INVALIDATION_NONE);
		}
		this->EndDrawing(target);
	}
}

void Drawable::Invalidate(INVALIDATION_MODE mode /*= INVALIDATION_DEFAULT*/)
{
	mInvalidationMode = mode;
}

INVALIDATION_MODE Drawable::GetInvalidationMode()
{
	return mInvalidationMode;
}

bool Drawable::IsValid()
{
	return mInvalidationMode == INVALIDATION_NONE;
}
