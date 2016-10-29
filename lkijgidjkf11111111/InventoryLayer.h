#pragma once
#include "Layer.h"
#include "Inventory.h"
#include "LinearTimedAnimation.h"
#include "ButtonLayer.h"
#include "InventoryItemLayer.h"
#include "MyGameItems.h"

class InventoryLayer :
	public Layer
{
protected:
	Inventory& mInventory;
	ButtonLayer *l_btnClose;

	InventoryItemLayer *l_item1;

	LinearTimedAnimation anim_showhide;
public:
	InventoryLayer(Game& game, int width, int height, Inventory& inv);
	virtual ~InventoryLayer();

	virtual void OnUpdate() override;
	virtual bool OnDraw(ID2D1RenderTarget* target) override;

	virtual void Initialize() override;

	void Show();
	void Hide();
};

