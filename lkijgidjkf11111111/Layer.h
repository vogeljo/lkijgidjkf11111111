#pragma once
#include "D2Pool.h"
#include "Drawable.h"

#include <set>

#define MUST_CALL

class Layer abstract
	: public Drawable
{
protected:
	int mWidth, mHeight;
	int mPosX, mPosY;
	std::set<Layer*> mLayers;
	Layer *mParent;
	float mOpacity;

	void SetTarget(ID2D1RenderTarget *target);
public:
	Layer(int width, int height);

	// takes target, creates none.
	Layer(ID2D1RenderTarget* target);

	virtual ~Layer();

	void AddLayer(Layer *layer);
	void SetPosition(int x, int y);
	int GetPositionX();
	int GetPositionY();
	int GetWidth();
	int GetHeight();
	D2D1_RECT_F GetRectangle();
	D2D1_RECT_F GetBounds();
	Layer* GetParent();
	void SetParent(Layer* layer);
	float GetOpacity();
	void SetOpacity(float opacity);

	bool Intersects(int x, int y);

	virtual void Update() override;
	virtual void Draw(ID2D1RenderTarget* target) override;


	virtual bool IsBitmap() override;

	// returns false when the mouse is over a sublayer.
	virtual bool MUST_CALL OnMouseMove(int x, int y);

	// returns false when the mouse is over a sublayer.
	virtual bool MUST_CALL OnLMouseDown(int x, int y);
};

