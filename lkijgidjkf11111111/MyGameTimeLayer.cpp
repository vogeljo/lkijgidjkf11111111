#include "MyGameTimeLayer.h"

MyGameTimeLayer::MyGameTimeLayer(Game& game, int width, int height, MyGameTime& time)
	: Layer(game, width, height), mTime(time)
{
	this->SetPadding(5.0f);
}

MyGameTimeLayer::~MyGameTimeLayer()
{
}

void MyGameTimeLayer::OnUpdate()
{

}

bool MyGameTimeLayer::OnDraw(ID2D1RenderTarget* target)
{
	target->Clear(D2D1::ColorF(D2D1::ColorF::Black, 0.0f));
	D2Pool::PrintText(mTime.ToString(), target, D2Pool::GetFormat(D2PoolFont::NORMAL), this->GetContentRectangle(), D2Pool::GetSolidColorBrush(D2D1::ColorF::White), 26.0f, DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT_FAR);

	return false;
}
