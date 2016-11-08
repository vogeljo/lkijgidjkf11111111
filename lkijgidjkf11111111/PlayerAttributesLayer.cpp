#include "PlayerAttributesLayer.h"

PlayerAttributesLayer::PlayerAttributesLayer(Game& game, Player& player, int width, int height)
	: Layer(game, width, height), mPlayer(player)
{
	this->SetPadding(5.0f);
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
	target->Clear(D2D1::ColorF(D2D1::ColorF::Black, 0.7f));

	D2Pool::PrintText(D2Pool::FormatString(L"Attribute von %s", mPlayer.GetName().c_str()), target, D2Pool::GetFormat(D2PoolFont::NORMAL), this->GetContentRectangle(), D2Pool::GetSolidColorBrush(D2D1::ColorF::White), 24.0f, DWRITE_TEXT_ALIGNMENT_CENTER);
	return true;
}
