#include "InventoryLayer.h"
#include "D2Pool.h"
#include "MyGameItems.h"

InventoryLayer::InventoryLayer(Game& game, int width, int height, Inventory& inv)
	: Layer(game, width, height), mInventory(inv)
{

}

InventoryLayer::~InventoryLayer()
{
}

void InventoryLayer::Initialize()
{
	this->SetPadding(10.0f);

	l_btnClose = new ButtonLayer(this->GetGame(), 40, 40);
	l_btnClose->SetText(L"\x2716");
	l_btnClose->SetPosition(this->GetWidth() - l_btnClose->GetWidth() - 10, 10);
	l_btnClose->SetClickHandler([&]() {
		this->Hide();
	});

	l_item1 = new InventoryItemLayer(this->GetGame(), mInventory, MyGameItems::APPLE);
	l_item1->SetPosition(50, 50);

	this->AddLayer(l_btnClose);
	this->AddLayer(l_item1);
}

void InventoryLayer::Show()
{
	this->SetVisible(true);
	anim_showhide.Start(0.0f, 1.0f, 60);
	this->Invalidate(INVALIDATION_NOCHILDREN);
}

void InventoryLayer::Hide()
{
	if (!anim_showhide.IsRunning()) {
		anim_showhide.Start(1.0f, 0.0f, 200, [&]() {
			this->SetVisible(false);
		});
	}
}

#include <iostream>

void InventoryLayer::OnUpdate()
{
	this->SetOpacity(anim_showhide.GetValue());
}

bool InventoryLayer::OnDraw(ID2D1RenderTarget* target)
{
	target->Clear(D2D1::ColorF(D2D1::ColorF::Black, 0.80f));

	D2Pool::PrintText(D2Pool::FormatString(L"Inventar (%n)", mInventory.GetTotalCount()), target, D2Pool::GetFormat(D2PoolFont::NORMAL), this->GetContentRectangle(), D2Pool::GetSolidColorBrush(D2D1::ColorF::White), 30.0f, DWRITE_TEXT_ALIGNMENT_LEADING);
	D2Pool::PrintText(L"Im Inventar kannst du Gegenstände aufbewahren.", target, D2Pool::GetFormat(D2PoolFont::NORMAL), this->GetContentRectangle(), D2Pool::GetSolidColorBrush(D2D1::ColorF::White), 18.0f, DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	return true;
}