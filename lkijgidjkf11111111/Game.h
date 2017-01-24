#pragma once
#include "Layer.h"
#include "Util.h"

#include "native.h"

enum class CGameSpecialDimensions {
	FULLSCREEN
};

class Game abstract
	: public Layer
{
protected:
	HWND mWindow;
	ID2D1HwndRenderTarget *mWindowTarget;

	Layer *mMouseHover;
	Layer *mFocused;
public:
	Game(int width, int height);
	virtual ~Game();

	HWND GetWindowHandle();

	void BindWindow(HWND handle);

	int GetDisplayWidth();
	int GetDisplayHeight();

	int GetMousePosX();
	int GetMousePosY();
	void SetFocus(Layer *layer);

	void Show();
	void SetFullscreen();

	void YieldFocus(Layer *from);

	virtual void Exit();

	virtual void Draw(ID2D1RenderTarget* target) override;

	virtual void OnUpdate() override;
	virtual bool OnDraw(ID2D1RenderTarget* target) override;


	virtual void OnKeyDown(int key) override;

	virtual bool MUST_CALL OnMouseMove(int x, int y) override;


	virtual void MUST_CALL OnKeyChar(wchar_t c) override;

	Layer* GetFocused();
};

