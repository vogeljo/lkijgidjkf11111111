#pragma once
#include <d2d1.h>
#include "Layer.h"

class CGame abstract
	: public Layer
{
protected:
	HWND mWindow;
	ID2D1HwndRenderTarget *mWindowTarget;
public:
	CGame(int width, int height);
	//CGame(HWND window);
	virtual ~CGame();

	HWND GetWindowHandle();

	void BindWindow(HWND handle);

	int GetDisplayWidth();
	int GetDisplayHeight();

	int GetMousePosX();
	int GetMousePosY();

	void Show();

	virtual void Draw(ID2D1RenderTarget* target) override;

	virtual void OnUpdate() override;
	virtual bool OnDraw(ID2D1RenderTarget* target) override;

};

