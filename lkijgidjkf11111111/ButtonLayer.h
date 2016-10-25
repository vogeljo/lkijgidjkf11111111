#pragma once
#include "Layer.h"
#include <string>
#include <functional>

class ButtonLayer :
	public Layer
{
	typedef std::function<void()> ClickHandler;
private:
	std::wstring mText;
	bool mHover;

	ClickHandler mClickHandler;
public:
	ButtonLayer(Game& game, int width, int height);
	virtual ~ButtonLayer();

	virtual void OnUpdate() override;
	virtual bool OnDraw(ID2D1RenderTarget* target) override;

	void SetText(std::wstring str);

	virtual void MUST_CALL OnMouseEnter() override;
	virtual void MUST_CALL OnMouseLeave() override;
	virtual bool MUST_CALL OnMouseMove(int x, int y) override;
	virtual bool MUST_CALL OnLMouseDown(int x, int y) override;

	void SetClickHandler(ClickHandler handler);
};

