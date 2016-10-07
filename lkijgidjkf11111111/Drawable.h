#pragma once
#include "D2Pool.h"

enum INVALIDATION_MODE : int {
	INVALIDATION_NONE,
	INVALIDATION_NOCHILDREN,
	INVALIDATION_ALL
};

#define INVALIDATION_DEFAULT INVALIDATION_MODE::INVALIDATION_ALL

class Drawable abstract
{
protected:
	ID2D1RenderTarget* mTarget;
	INVALIDATION_MODE mInvalidationMode;
	bool mInitalized;
	int mIsDrawing;

protected:
	void StartDrawing(ID2D1RenderTarget *target);
	void EndDrawing(ID2D1RenderTarget *target);
public:
	Drawable(ID2D1RenderTarget* target);
	virtual ~Drawable();

	virtual bool IsBitmap();
	virtual void Initialize();

	virtual ID2D1Bitmap* GetBitmap();

	virtual ID2D1RenderTarget* GetTarget();

	virtual void OnUpdate() = 0;
	virtual bool OnDraw(ID2D1RenderTarget* target) = 0;

	virtual void Update();
	virtual void Draw(ID2D1RenderTarget* target);

	void Invalidate(INVALIDATION_MODE mode = INVALIDATION_DEFAULT);
	INVALIDATION_MODE GetInvalidationMode();
	bool IsValid();
};

