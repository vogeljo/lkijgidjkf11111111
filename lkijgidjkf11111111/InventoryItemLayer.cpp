#include "InventoryItemLayer.h"

#include "Inventory.h"


InventoryItemLayer::InventoryItemLayer(Game& game, Inventory& inventory, ItemToken token)
	: Layer(game, 80, 80), mBackground(D2D1::ColorF::Black), mInventory(inventory)
{
	this->SetInventory(inventory);
	this->SetToken(token);
	this->SetPadding(8.0f);
}

InventoryItemLayer::~InventoryItemLayer()
{
}

void InventoryItemLayer::SetToken(ItemToken token)
{
	mToken = token;
	this->Invalidate(INVALIDATION_NOCHILDREN);
}

void InventoryItemLayer::SetInventory(Inventory& inventory)
{
	mInventory = inventory;
	this->Invalidate(INVALIDATION_NOCHILDREN);
}

void InventoryItemLayer::OnUpdate()
{
	int v = mInventory.Get(mToken);
	if (mLastValue != v)
		this->Invalidate(INVALIDATION_NOCHILDREN);
	mLastValue = v;
}

bool InventoryItemLayer::OnDraw(ID2D1RenderTarget* target)
{
	D2D1_COLOR_F clearColor = D2D1::ColorF(D2D1::ColorF::Black);
	target->Clear(clearColor);

	auto item = ItemPool::Get(mToken);

	target->DrawBitmap(D2Pool::GetGraphic(item->GetGraphic()), this->GetContentRectangle());
	D2Pool::PrintText(std::to_wstring(mInventory.Get(mToken)), target, D2Pool::GetFormat(D2PoolFont::NORMAL), this->GetContentRectangle(), D2Pool::GetSolidColorBrush(D2Pool::GetReadableColor(clearColor)), 18.0f, DWRITE_TEXT_ALIGNMENT_TRAILING, DWRITE_PARAGRAPH_ALIGNMENT_FAR);

	target->DrawRectangle(this->GetBoundingRectangle(), D2Pool::GetSolidColorBrush(mBackground), 2.0f);

	return true;
}

void MUST_CALL InventoryItemLayer::OnMouseEnter()
{
	mBackground = D2D1::ColorF::White;
	this->Invalidate(INVALIDATION_NOCHILDREN);
}

void MUST_CALL InventoryItemLayer::OnMouseLeave()
{
	mBackground = D2D1::ColorF(D2D1::ColorF::Black);
	this->Invalidate(INVALIDATION_NOCHILDREN);
}
