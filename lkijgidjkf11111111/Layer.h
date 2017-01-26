#pragma once
#include "D2Pool.h"
#include "Drawable.h"

#include "TweenDynamic.h"

#include <vector>
#include <queue>

#define MUST_CALL

class Game;

class Layer abstract
	: public Drawable
{
protected:
	Game& mGame;
	int mWidth, mHeight;
	int mPosX, mPosY;
	std::vector<Layer*> mLayers;
	Layer *mParent;
	bool mVisible;
	bool mAlphaBackground;
	D2D1_RECT_F mPadding;
	bool mHideOnExitKey;

	// replace Drawable::mTarget by this.
	ID2D1BitmapRenderTarget *mBackBuffer;

	void SetTarget(ID2D1BitmapRenderTarget *target);

	// animations
	LinearTweenDynamic<float> dynOpacity;
public:
	Layer(Game& game, int width, int height);

	// takes target, creates none.
	//Layer(ID2D1RenderTarget* target);

	virtual ~Layer();

	Game& GetGame();
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
	bool IsOpaque();
	bool GetHideOnExitKey();
	void SetHideOnExitKey(bool value);

	bool Intersects(int x, int y);
	Layer* GetLayerAt(int x, int y);
	Layer* FindLayerAt(int x, int y);
	virtual bool TestMouseHit(int x, int y);
	bool IsTransparent();

	bool HasFocus();

	virtual void Update() override;
	virtual void Draw(ID2D1RenderTarget* target) override;

	void Print(ID2D1RenderTarget *target);

	void FadeIn(DWORD duration_ms = 100);
	void FadeOut(DWORD duration_ms = 100);

	virtual bool IsBitmap() override;
	virtual bool TakesFocus();

	virtual void OnExitKey();

	virtual void OnLayerAdded(Layer *layer);

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

	// returns false when the mouse is over a sublayer.
	virtual bool MUST_CALL OnVMouseScroll(int x, int y, float delta);
	// returns false when the mouse is over a sublayer.
	virtual bool MUST_CALL OnHMouseScroll(int x, int y, float delta);

	virtual void MUST_CALL OnKeyDown(int key);
	virtual void MUST_CALL OnKeyChar(wchar_t c);

	virtual void MUST_CALL OnMouseEnter();
	virtual void MUST_CALL OnMouseLeave();

	virtual void OnFocusChange(bool hasFocus);
	void Focus();

	// Show layer, taking focus
	void Show();

	// Hide layer, yielding focus
	void Hide();

	Layer* GetChildBehind(Layer *child);
};

