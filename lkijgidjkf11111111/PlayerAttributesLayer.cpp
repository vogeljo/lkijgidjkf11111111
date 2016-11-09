#include "PlayerAttributesLayer.h"

// Stats to list in this layer
Stat DISPLAY_STATS[] = {
	Stat::Money
};
#define DISPLAY_STATS_LENGTH (sizeof(DISPLAY_STATS) / sizeof(Stat))

PlayerAttributesLayer::PlayerAttributesLayer(Game& game, Player& player, int width, int height)
	: Layer(game, width, height), mPlayer(player)
{
	this->SetPadding(8.0f);
}

PlayerAttributesLayer::~PlayerAttributesLayer()
{
}

void PlayerAttributesLayer::SetPlayer(Player& player)
{
	mPlayer = player;
}

void PlayerAttributesLayer::OnUpdate()
{

}

bool PlayerAttributesLayer::OnDraw(ID2D1RenderTarget* target)
{
	target->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
	target->Clear(D2D1::ColorF(D2D1::ColorF::Black, 0.0f));

	target->FillRectangle(this->GetBoundingRectangle(), mBackground);

	D2Pool::PrintText(D2Pool::FormatString(L"Attribute von %s", mPlayer.GetName().c_str()), target, D2Pool::GetFormat(D2PoolFont::NORMAL), this->GetContentRectangle(), D2Pool::GetSolidColorBrush(D2D1::ColorF::White), 24.0f, DWRITE_TEXT_ALIGNMENT_CENTER);

	auto rect = this->GetContentRectangle();
	rect.top += 40.0f;

	// draw health bar
	int health = mPlayer.GetStats().Get(Stat::Health);

	D2Pool::PrintText(GetStatText(Stat::Health), target, D2Pool::GetFormat(D2PoolFont::NORMAL), rect, D2Pool::GetSolidColorBrush(D2D1::ColorF::White));
	D2Pool::PrintText(D2Pool::FormatString(L"%n", health), target, D2Pool::GetFormat(D2PoolFont::NORMAL), rect, D2Pool::GetSolidColorBrush(D2D1::ColorF::White), DWRITE_TEXT_ALIGNMENT_TRAILING);

	rect.top += 20.0f;
	float ratio = ((float)health / 100.0f);
	auto color = Util::TweenHealth(ratio);
	auto health_bar_rect = D2D1::RectF(rect.left, rect.top, rect.right, rect.top + 5.0f);
	health_bar_rect.right = health_bar_rect.left + (health_bar_rect.right - health_bar_rect.left) * ratio;
	target->FillRectangle(health_bar_rect, D2Pool::GetSolidColorBrush(color));

	rect.top += 20.0f;
	target->DrawLine(D2D1::Point2F(0, rect.top), D2D1::Point2F(this->GetWidth(), rect.top), D2Pool::GetSolidColorBrush(D2D1::ColorF::Gray));
	rect.top += 10.0f;

	for (int i = 0; i < DISPLAY_STATS_LENGTH; ++i) {

		Stat s = DISPLAY_STATS[i];
		int val = mPlayer.GetStats().Get(s);

		D2Pool::PrintText(GetStatText(s), target, D2Pool::GetFormat(D2PoolFont::NORMAL), rect, D2Pool::GetSolidColorBrush(D2D1::ColorF::White), 17.0f, DWRITE_TEXT_ALIGNMENT_LEADING);
		D2Pool::PrintText(D2Pool::FormatString(L"%n", val), target, D2Pool::GetFormat(D2PoolFont::NORMAL), rect, D2Pool::GetSolidColorBrush(D2D1::ColorF::White), 17.0f, DWRITE_TEXT_ALIGNMENT_TRAILING);

		rect.top += 30.0f;
	}

	// TODO: catch changes in attributes
	return false;
}

void PlayerAttributesLayer::Initialize()
{
	Layer::Initialize();
	
	D2D1_GRADIENT_STOP g_stops[] = {
		D2D1::GradientStop(0.0f, D2D1::ColorF(D2D1::ColorF::Black, 0.3f)),
		D2D1::GradientStop(0.9f, D2D1::ColorF(D2D1::ColorF::Black, 0.2f)),
		D2D1::GradientStop(1.0f, D2D1::ColorF(D2D1::ColorF::Black, 0.5f))
	};
	ID2D1GradientStopCollection *stops = nullptr;
	D2Pool::GetSourceRenderTarget()->CreateGradientStopCollection(g_stops, 3, D2D1_GAMMA_1_0, D2D1_EXTEND_MODE::D2D1_EXTEND_MODE_CLAMP, &stops);

	D2Pool::GetSourceRenderTarget()->CreateLinearGradientBrush(D2D1::LinearGradientBrushProperties(D2D1::Point2F(0, 0), D2D1::Point2F(this->GetWidth(), 0)), stops, &mBackground);

	stops->Release();
}
