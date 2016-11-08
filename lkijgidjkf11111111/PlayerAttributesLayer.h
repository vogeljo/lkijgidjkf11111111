#pragma once
#include "Layer.h"
#include "Player.h"

#include "D2Pool.h"

class PlayerAttributesLayer :
	public Layer
{
private:
	Player& mPlayer;
public:
	PlayerAttributesLayer(Game& game, Player& player, int width, int height);
	virtual ~PlayerAttributesLayer();

	void SetPlayer(Player& player);

	virtual void OnUpdate() override;
	virtual bool OnDraw(ID2D1RenderTarget* target) override;

};

