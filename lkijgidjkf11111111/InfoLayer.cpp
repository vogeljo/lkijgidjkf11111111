#include "InfoLayer.h"

InfoLayer::InfoLayer(Game& game, int width, int height)
	: Layer(game, width, height)
{
	this->SetVisible(false);
	this->SetBackground(D2D1::ColorF(D2D1::ColorF::Gray));

#define PADDING 8.0f
	this->SetPadding(PADDING);
}

InfoLayer::~InfoLayer()
{
}

void InfoLayer::Initialize()
{
	IDWriteRenderingParams *params;
	D2Pool::GetWriteFactory()->CreateCustomRenderingParams(1.0f, 0.0f, 1.0f, DWRITE_PIXEL_GEOMETRY_RGB, DWRITE_RENDERING_MODE_CLEARTYPE_GDI_NATURAL, &params);
	//this->GetTarget()->SetTextRenderingParams(params);

	this->GetTarget()->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
}

void InfoLayer::SetBackground(D2D1_COLOR_F color)
{
	mBackground = color;
	this->Invalidate();
}

bool InfoLayer::TestMouseHit(int x, int y)
{
	return false;
}

bool InfoLayer::TakesFocus()
{
	return false;
}

void InfoLayer::OnUpdate()
{

}

bool InfoLayer::OnDraw(ID2D1RenderTarget* target)
{
	target->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
	target->Clear(mBackground);

	target->DrawRectangle(this->GetBoundingRectangle(), D2Pool::GetSolidColorBrush(D2D1::ColorF::Black), 2.0f);

	auto rect = this->GetContentRectangle();
	
	D2Pool::PrintText(mTitle, target, D2Pool::GetFormat(D2PoolFont::NORMAL), rect, D2Pool::GetSolidColorBrush(D2D1::ColorF::Black), 16.0f, DWRITE_FONT_WEIGHT_BOLD, DWRITE_TEXT_ALIGNMENT_CENTER);

	rect.top += 20;

	D2Pool::PrintText(mText, target, D2Pool::GetFormat(D2PoolFont::NORMAL), rect, D2Pool::GetSolidColorBrush(D2D1::ColorF::Black));
	return true;
}

void InfoLayer::SetText(std::wstring str)
{
	mText = str;
	this->Invalidate();
}

void InfoLayer::SetTitle(std::wstring str)
{
	mTitle = str;
	this->Invalidate();
}