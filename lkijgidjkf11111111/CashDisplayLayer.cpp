#include "CashDisplayLayer.h"

#include "D2Pool.h"

CashDisplayLayer::CashDisplayLayer(int value, int width, int height)
	: Layer(width, height)
{
	this->SetValue(value);
}

CashDisplayLayer::~CashDisplayLayer()
{
}

void CashDisplayLayer::SetValue(int value)
{
	mValue = value;
}

void CashDisplayLayer::OnUpdate()
{
	//throw std::logic_error("The method or operation is not implemented.");
}

bool CashDisplayLayer::OnDraw(ID2D1RenderTarget* target)
{
	target->Clear(D2D1::ColorF(D2D1::ColorF::Black));

	D2Pool::PrintText(D2Pool::FormatString(L"%n€", mValue), target, D2Pool::GetFormat(D2PoolFont::NORMAL), this->GetRectangle(), D2Pool::GetSolidColorBrush(D2D1::ColorF::White), DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	return true;
}
