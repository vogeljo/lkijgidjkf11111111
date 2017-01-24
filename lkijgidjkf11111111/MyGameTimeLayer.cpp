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
	D2Pool::PrintText(D2Pool::FormatString(L"Tag %n", mTime.GetDay()), target, D2Pool::GetFormat(D2PoolFont::NORMAL), this->GetContentRectangle(), D2Pool::GetSolidColorBrush(D2D1::ColorF::White), 24.0f, DWRITE_TEXT_ALIGNMENT_TRAILING, DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
	D2Pool::PrintText(mTime.ToString(), target, D2Pool::GetFormat(D2PoolFont::NORMAL), this->GetContentRectangle() + D2D1::Point2F(0.0f, 20.0f), D2Pool::GetSolidColorBrush(D2D1::ColorF::White), 24.0f, DWRITE_TEXT_ALIGNMENT_TRAILING, DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

	auto clock_rect = this->GetContentRectangle();
	clock_rect.top += 30;
	auto center = D2D1::Point2F(clock_rect.left + (clock_rect.right - clock_rect.left) / 2, clock_rect.top + (clock_rect.bottom - clock_rect.top) / 2);
	float radius = std::min(clock_rect.right - clock_rect.left, clock_rect.bottom - clock_rect.top) / 2 * (4.0f / 5.0f);
	
	float fseconds = (float)mTime.GetSecond();
	float fmins = (float)mTime.GetMinute() + fseconds / 60.0f;
	int ihour = mTime.GetHour();
	float fhours = (float)(ihour % 12) + fmins / 60.0f;

	float minute = fmins / 60.0f * M_PI * 2;
	float hour = fhours / 12.0f * M_PI * 2;
	float hour_total = (float)ihour + fmins / 60.0f;

	float day_progress = hour_total / 24.0f;
	float day_hue = pow(sin(hour_total * (M_PI_2) / 12), 4);

	auto color_clock = D2D1::ColorF(day_hue, 0.0f, 1.0f - day_hue);

	auto ell = D2D1::Ellipse(center, radius, radius);

	target->FillEllipse(ell, D2Pool::GetSolidColorBrush(color_clock, 0.3f));
	target->DrawEllipse(ell, D2Pool::GetSolidColorBrush(D2D1::ColorF::White), 3.0f);

	int n = 0;
	for (float m = 0.0f; m < 60.0f; m += 5.0f, ++n) {
		auto p1 = D2D1::Point2F(center.x + sinf(m / 60.0f * M_PI * 2) * radius, center.y - cosf(m / 60.0f * M_PI * 2) * radius);
		auto p2 = D2D1::Point2F(center.x + sinf(m / 60.0f * M_PI * 2) * radius * 0.8f, center.y - cosf(m / 60.0f * M_PI * 2) * radius * 0.8f);
		target->DrawLine(p1, p2, D2Pool::GetSolidColorBrush(D2D1::ColorF::White), n % 3 == 0 ? 2.5f : 1.0f);
	}

	target->DrawLine(center, D2D1::Point2F(center.x + sinf(hour) * radius * 3 / 5, center.y - cosf(hour) * radius * 3 / 5), D2Pool::GetSolidColorBrush(D2D1::ColorF::White), 4.0f);
	target->DrawLine(D2D1::Point2F(center.x - sinf(minute) * radius * 2 / 10, center.y + cosf(minute) * radius * 2 / 10), D2D1::Point2F(center.x + sinf(minute) * radius * 9 / 10, center.y - cosf(minute) * radius * 9 / 10), D2Pool::GetSolidColorBrush(D2D1::ColorF::White), 2.0f);

	target->FillEllipse(D2D1::Ellipse(center, 5.0f, 5.0f), D2Pool::GetSolidColorBrush(D2D1::ColorF::LightGray));

	return false;
}

bool MyGameTimeLayer::TestMouseHit(int x, int y)
{
	return false;
}

void MyGameTimeLayer::Initialize()
{

}

bool MyGameTimeLayer::TakesFocus()
{
	return false;
}
