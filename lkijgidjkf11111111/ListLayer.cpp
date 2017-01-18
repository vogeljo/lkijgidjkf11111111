#include "ListLayer.h"

ListLayer::~ListLayer()
{
}

void ListLayer::DrawListItem(ID2D1RenderTarget *target, ListItem& item, D2D1_RECT_F& rect)
{
	auto w = rect.right - rect.left;
	auto h = rect.bottom - rect.top;

	target->FillRectangle(rect, D2Pool::GetSolidColorBrush(D2D1::ColorF::Black));
	D2Pool::PrintText(item.text, target, D2Pool::GetFormat(D2PoolFont::NORMAL), rect, D2Pool::GetSolidColorBrush(D2D1::ColorF::White),mFontSize, DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

}


bool ListLayer::OnDraw(ID2D1RenderTarget* target)
{
	const float itemHeight = 30.0f;
	const float gap = 5.0f;

	float posY = gap;

	target->Clear(D2D1::ColorF(D2D1::ColorF::Black));


	for each (auto item in mItems) {
		auto r = this->GetContentRectangle();
		r.top = posY;
		r.bottom = posY + itemHeight;

		this->DrawListItem(target, item, r);

		posY += itemHeight + gap;
	}

	return true;
}

void ListLayer::OnUpdate()
{

}

void ListLayer::SetFontSize(float size)
{
	mFontSize = size;

	this->Invalidate();
}

size_t ListLayer::AddItem(std::wstring text)
{
	return this->AddItem(text, nullptr);
}

size_t ListLayer::AddItem(std::wstring text, void *data)
{
	mItems.push_back({ text, data });

	this->Invalidate();
	return mItems.size() - 1;
}

void ListLayer::RemoveItem(size_t index)
{
	auto it = mItems.begin();
	std::advance(it, index);
	mItems.erase(it);

	this->Invalidate();
}

void ListLayer::Clear()
{
	mItems.clear();
	this->Invalidate();
}

void ListLayer::Initialize()
{
	this->SetFontSize(18.0f);
	this->SetPadding(5.0f);
}
