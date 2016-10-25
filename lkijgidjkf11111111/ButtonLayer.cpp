#include "ButtonLayer.h"

ButtonLayer::ButtonLayer(int width, int height)
	: Layer(width, height), mHover(false), mClickHandler(nullptr)
{
	this->SetText(L"Button");
}

ButtonLayer::~ButtonLayer()
{
}

void ButtonLayer::OnUpdate()
{

}

bool ButtonLayer::OnDraw(ID2D1RenderTarget* target)
{
	target->Clear(D2D1::ColorF(D2D1::ColorF::Black, 0.0f));

	auto rrect = D2D1::RoundedRect(this->GetBoundingRectangle(), 5.0f, 5.0f);
	rrect.rect.top += 1;
	rrect.rect.left += 1;
	rrect.rect.right -= 1;
	rrect.rect.bottom -= 1;

	target->FillRoundedRectangle(rrect, D2Pool::GetSolidColorBrush(mHover ? D2D1::ColorF::Gray : D2D1::ColorF::White));
	target->DrawRoundedRectangle(rrect, D2Pool::GetSolidColorBrush(D2D1::ColorF::Black), 1.0f);
	D2Pool::PrintText(mText, target, D2Pool::GetFormat(D2PoolFont::NORMAL), this->GetContentRectangle(), D2Pool::GetSolidColorBrush(D2D1::ColorF::Black), DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	return true;
}

void ButtonLayer::SetText(std::wstring str)
{
	mText = str;
	this->Invalidate(INVALIDATION_NOCHILDREN);
}

void MUST_CALL ButtonLayer::OnMouseEnter()
{
	mHover = true;
	this->Invalidate(INVALIDATION_NOCHILDREN);
}

void MUST_CALL ButtonLayer::OnMouseLeave()
{
	mHover = false;
	this->Invalidate(INVALIDATION_NOCHILDREN);
}

bool MUST_CALL ButtonLayer::OnMouseMove(int x, int y)
{
	SetCursor(LoadCursor(NULL, IDC_HAND));
	return Layer::OnMouseMove(x, y);
}

bool MUST_CALL ButtonLayer::OnLMouseDown(int x, int y)
{
	if (mClickHandler)
		mClickHandler();
	return Layer::OnLMouseDown(x, y);
}

void ButtonLayer::SetClickHandler(ClickHandler handler)
{
	mClickHandler = handler;
}
