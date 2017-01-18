#include "HouseLayer.h"

HouseLayer::HouseLayer(HouseInfo *house, Game& game, int width, int height)
	: Layer(game, width, height), mHouse(house)
{
	this->SetPadding(20.0f);
}

HouseLayer::~HouseLayer()
{
}

void HouseLayer::OnUpdate()
{

}

bool HouseLayer::OnDraw(ID2D1RenderTarget* target)
{
	target->Clear(D2D1::ColorF(D2D1::ColorF::Black, 0.8f));

	if (mHouse) {
		D2Pool::PrintText(mHouse->GetName(), target, D2Pool::GetFormat(D2PoolFont::NORMAL), this->GetContentRectangle(), D2Pool::GetSolidColorBrush(D2D1::ColorF::White), 24.0f, DWRITE_FONT_WEIGHT_BOLD, DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		D2Pool::PrintText(L"Work in progress ...", target, D2Pool::GetFormat(D2PoolFont::MONOSPACE), this->GetContentRectangle(), D2Pool::GetSolidColorBrush(D2D1::ColorF::White), 16.0f, DWRITE_FONT_WEIGHT_EXTRA_LIGHT, DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	}
	else {
		D2Pool::PrintText(L"No house selected.", target, D2Pool::GetFormat(D2PoolFont::NORMAL), this->GetContentRectangle(), D2Pool::GetSolidColorBrush(D2D1::ColorF::White), DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	}

	return true;
}

void HouseLayer::SetHouse(HouseInfo *info)
{
	mHouse = info;
	this->Invalidate();
}
