#pragma once
#include <d2d1.h>
#include "Layer.h"

class CGame abstract
	: public Layer
{
protected:
	HWND mWindow;
public:
	CGame(int width, int height);
	virtual ~CGame();

	HWND GetWindowHandle();

	void BindWindow(HWND handle);

	int GetMousePosX();
	int GetMousePosY();

	void Show();
};

