#pragma once
#include "D2Pool.h"
#include "Drawable.h"

#include <vector>

#define MUST_CALL

class Game;

class Layer abstract
	: public Drawable
{
protected:
	int mWidth, mHeight;
	int mPosX, mPosY;
	std::vector<Layer*> mLayers;
	Layer *mParent;
	float mOpacity;
	bool mVisible;
	bool mAlphaBackground;
	D2D1_RECT_F mPadding;

	void SetTarget(ID2D1BitmapRenderTarget *target);
	void DrawFromBackBuffer(ID2D1RenderTarget *target, D2D1_RECT_F& rect);
public:
	Layer(int width, int height);

	// takes target, creates none.
	//Layer(ID2D1RenderTarget* target);

	virtual ~Layer();

	void AddLayer(Layer *layer);
	void AddLayer(Layer *layer, Layer *lower);
	void SetPosition(int x, int y);
	int GetPositionX();
	int GetPositionY();
	int GetWidth();
	int GetHeight();
	D2D1_RECT_F GetContentRectangle();
	D2D1_RECT_F GetBoundingRectangle();
	D2D1_RECT_F GetBounds();
	Layer* GetParent();
	void SetParent(Layer* layer);
	void InvalidateParent();
	float GetOpacity();
	void SetOpacity(float opacity);
	void SetVisible(bool value);
	void SetPadding(float padding);
	void SetPadding(float left, float top, float right, float bottom);
	bool IsVisible();

	bool Intersects(int x, int y);
	Layer* GetLayerAt(int x, int y);
	Layer* FindLayerAt(int x, int y);
	virtual bool TestMouseHit(int x, int y);
	bool IsTransparent();

	virtual void Update() override;
	virtual void Draw(ID2D1RenderTarget* target) override;


	virtual bool IsBitmap() override;

	// returns false when the mouse is over a sublayer.
	virtual bool MUST_CALL OnMouseMove(int x, int y);

	// returns false when the mouse is over a sublayer.
	virtual bool MUST_CALL OnLMouseDown(int x, int y);

	// returns false when the mouse is over a sublayer.
	virtual bool MUST_CALL OnRMouseDown(int x, int y);

	// returns false when the mouse is over a sublayer.
	virtual bool MUST_CALL OnLMouseUp(int x, int y);

	// returns false when the mouse is over a sublayer.
	virtual bool MUST_CALL OnRMouseUp(int x, int y);

	virtual void MUST_CALL OnKeyDown(int key);

	virtual void MUST_CALL OnMouseEnter();
	virtual void MUST_CALL OnMouseLeave();
};

