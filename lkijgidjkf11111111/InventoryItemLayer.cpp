#include "InventoryItemLayer.h"

#include "Inventory.h"

#define INVENTORY_ITEM_LAYER_SIZE 80

InventoryItemLayer::InventoryItemLayer(Game& game, Inventory& inventory, ItemToken token)
	: Layer(game, INVENTORY_ITEM_LAYER_SIZE, INVENTORY_ITEM_LAYER_SIZE), mBackground(D2D1::ColorF::Black), mInventory(inventory)
{
	this->SetInventory(inventory);
	this->SetToken(token);
	this->SetPadding(8.0f);

	float radius = this->GetWidth() / 2;

	/*D2D1_GRADIENT_STOP stops[] = {
		D2D1::GradientStop(0.0f, D2D1::ColorF(D2D1::ColorF::Black, 1.0f)),
		D2D1::GradientStop(1.0f, D2D1::ColorF(D2D1::ColorF::Black, 0.0f))
	};
	ID2D1GradientStopCollection *coll = nullptr;
	D2Pool::GetSourceRenderTarget()->CreateGradientStopCollection(stops, 2, D2D1_GAMMA_2_2, D2D1_EXTEND_MODE::D2D1_EXTEND_MODE_CLAMP, &coll);

	D2Pool::GetSourceRenderTarget()->CreateRadialGradientBrush(D2D1::RadialGradientBrushProperties(D2D1::Point2F(radius, radius), D2D1::Point2F(), radius, radius), coll, &mBackBrush);*/
}

InventoryItemLayer::~InventoryItemLayer()
{
}

void InventoryItemLayer::SetToken(ItemToken token)
{
	mToken = token;
	this->Invalidate();
}

void InventoryItemLayer::SetInventory(Inventory& inventory)
{
	mInventory = inventory;
	this->Invalidate();
}

void InventoryItemLayer::OnUpdate()
{
	this->InvalidateParent();

	int v = mInventory.Get(mToken);
	if (mLastValue != v)
		this->Invalidate();
	mLastValue = v;
}

bool InventoryItemLayer::OnDraw(ID2D1RenderTarget* target)
{
	D2D1_COLOR_F clearColor = D2D1::ColorF(D2D1::ColorF::White, 0.0f);
	target->Clear(clearColor);

	float radius = INVENTORY_ITEM_LAYER_SIZE / 2;

	target->FillEllipse(D2D1::Ellipse(D2D1::Point2F(radius, radius), radius, radius), D2Pool::GetSolidColorBrush(mBackground));

	Item* it = ItemPool::Get(mToken);
	auto g = D2Pool::GetGraphic(it->GetGraphic());

	target->DrawBitmap(g, this->GetContentRectangle());
	D2Pool::PrintText(std::to_wstring(mInventory.Get(mToken)), target, D2Pool::GetFormat(D2PoolFont::NORMAL), this->GetContentRectangle(), D2Pool::GetSolidColorBrush(D2Pool::GetReadableColor(clearColor)), 18.0f, DWRITE_TEXT_ALIGNMENT_TRAILING, DWRITE_PARAGRAPH_ALIGNMENT_FAR);

	//target->DrawRectangle(this->GetBoundingRectangle(), D2Pool::GetSolidColorBrush(mBackground), 2.0f);
	return true;
}

bool InventoryItemLayer::TestMouseHit(int x, int y)
{
	x -= this->GetPositionX();
	y -= this->GetPositionY();

	float radius = INVENTORY_ITEM_LAYER_SIZE / 2;
	float dx = radius - x;
	float dy = radius - y;
	float dist = std::sqrtf(dx * dx + dy * dy);

	return dist <= (INVENTORY_ITEM_LAYER_SIZE / 2);
}

void MUST_CALL InventoryItemLayer::OnMouseEnter()
{
	mBackground = D2D1::ColorF::White;
	this->Invalidate();
}

void MUST_CALL InventoryItemLayer::OnMouseLeave()
{
	mBackground = D2D1::ColorF(D2D1::ColorF::Black);
	this->Invalidate();
}

bool MUST_CALL InventoryItemLayer::OnMouseMove(int x, int y)
{
	SetCursor(LoadCursor(NULL, IDC_HAND));
	return Layer::OnMouseMove(x, y);
}
