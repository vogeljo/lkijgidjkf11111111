#pragma once
#include "Layer.h"
#include "AlternatingDynamic.h"

#include <cctype>

class ConsoleLayer :
	public Layer
{
private:
	std::wstring mLine;

	AlternatingDynamic<int> dyn_caret = AlternatingDynamic<int>(0, 1, 500);
	int last_caret_dyn_value;
public:
	using Layer::Layer;
	virtual ~ConsoleLayer();

	virtual void OnUpdate() override;
	virtual bool OnDraw(ID2D1RenderTarget* target) override;

	virtual void MUST_CALL OnKeyChar(wchar_t c) override;
	virtual void OnFocusChange(bool hasFocus) override;

	virtual void Initialize() override;

	void OnCommand(std::wstring cmd);
};

