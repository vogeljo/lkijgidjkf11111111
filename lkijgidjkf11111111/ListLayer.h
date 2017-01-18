#pragma once
#include "Layer.h"
#include <list>

class ListLayer :
	public Layer
{
private:
	struct ListItem {
		std::wstring text;
		void *data;
	};

	void DrawListItem(ID2D1RenderTarget *target, ListItem& item, D2D1_RECT_F& rect);

	std::list<ListItem> mItems;
	float mFontSize;
public:
	using Layer::Layer;
	virtual ~ListLayer();

	virtual bool OnDraw(ID2D1RenderTarget* target) override;
	virtual void OnUpdate() override;

	void SetFontSize(float size);

	size_t AddItem(std::wstring text);
	size_t AddItem(std::wstring text, void *data);
	void RemoveItem(size_t index);
	void Clear();

	virtual void Initialize() override;

};

