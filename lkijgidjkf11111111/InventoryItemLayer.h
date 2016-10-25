#pragma once
#include "Layer.h"
#include "Item.h"

class Inventory;

class InventoryItemLayer :
	public Layer
{
private:
	ItemToken mToken;
	D2D1::ColorF mBackground;
	int mLastValue;

	Inventory& mInventory;
public:
	InventoryItemLayer(Game& game, Inventory& inventory, ItemToken token);
	virtual ~InventoryItemLayer();

	void SetToken(ItemToken token);
	void SetInventory(Inventory& inventory);

	virtual void OnUpdate() override;
	virtual bool OnDraw(ID2D1RenderTarget* target) override;


	virtual void MUST_CALL OnMouseEnter() override;


	virtual void MUST_CALL OnMouseLeave() override;

};

