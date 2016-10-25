#pragma once
#include "Layer.h"
#include <string>
#include "LinearTimedAnimation.h"

class InfoLayer :
	public Layer
{
protected:
	std::wstring mText, mTitle;
	D2D1_COLOR_F mBackground;

	LinearTimedAnimation mAnimation;
public:
	InfoLayer(int width, int height);
	virtual ~InfoLayer();

	virtual void OnUpdate() override;
	virtual bool OnDraw(ID2D1RenderTarget* target) override;

	void SetText(std::wstring str);
	void SetTitle(std::wstring str);

	virtual void Initialize() override;

	void SetBackground(D2D1_COLOR_F color);

	virtual bool TestMouseHit(int x, int y) override;

	void Show();
	void Hide();

};

