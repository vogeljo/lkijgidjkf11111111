#include "MapLayer.h"
#include "MyGame.h"
#include <cmath>

D2D1_RECT_F MapLayer::GetRectForCell(int cellX, int cellY)
{
	float x = cellX * mTileSize - mDragScroll.GetOffsetX();
	float y = cellY * mTileSize - mDragScroll.GetOffsetY();
	return D2D1::RectF(x, y, x + mTileSize, y + mTileSize);
}

int MapLayer::GetTileForXY(int x_y)
{
	return floor((float)(x_y) / (float)mTileSize);
}

MapLayer::MapLayer(int width, int height, MyGame& game, MapData& mapData)
	: Layer(width, height), mMouseTileX(0), mMouseTileY(0), mDragScroll(width, height), mMapData(mapData), mPlayer(nullptr), mGame(game)
{
	this->SetTileSize(32);
	this->SetPadding(5.0f);

	mDragScroll.SetDragAreaSize(this->mTileSize * mapData.GetWidth() - 1, this->mTileSize * mapData.GetHeight() - 1);
}

MapLayer::~MapLayer()
{
}

void MapLayer::Initialize()
{
	l_info = new InfoLayer(300, 100);
	this->AddLayer(l_info);
	srand(GetTickCount());
}

void MapLayer::OnUpdate()
{
	++nUpdate;

	auto loc = mPlayer->GetLocation();
	bool changed = false;
	float speed = 0.1f;

	if (Util::IsKeyDown('W')) {
		loc.y -= speed;
		changed = true;
	}
	if (Util::IsKeyDown('A')) {
		loc.x -= speed;
		changed = true;
	}
	if (Util::IsKeyDown('S')) {
		loc.y += speed;
		changed = true;
	}
	if (Util::IsKeyDown('D')) {
		loc.x += speed;
		changed = true;
	}
	if (Util::IsKeyDown(VK_SPACE)) {
		this->CenterUnit(mPlayer);
	}

	//loc.x += (float)(rand() % 400 - 200) / 1000.0f;
	//loc.y += (float)(rand() % 400 - 200) / 1000.0f;
	//changed = true;

	if (changed) {
		mPlayer->SetLocation(loc.x, loc.y);
		this->Invalidate(INVALIDATION_NOCHILDREN);
	}
}

bool MapLayer::OnDraw(ID2D1RenderTarget* target)
{
	target->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
	target->Clear(D2D1::ColorF(D2D1::ColorF::DarkGreen));

	int off_x = mDragScroll.GetOffsetX();
	int off_y = mDragScroll.GetOffsetY();

	int gridOffsetX = off_x % mTileSize;
	int gridOffsetY = off_y % mTileSize;

	int first_x = mDragScroll.GetOffsetX() / mTileSize;
	int first_y = mDragScroll.GetOffsetY() / mTileSize;

	int last_x = first_x + (this->GetWidth() / mTileSize);
	int last_y = first_y + (this->GetHeight() / mTileSize);

	// draw tiles
	for (int x = first_x; x <= last_x; ++x) {
		for (int y = first_y; y <= last_y; ++y) {
			auto type = mMapData.GetType(MapCoordPair(x, y));
			auto color = GetMapTileColor(type);
			bool draw = type != MapTileType::None;

			if (draw)
				target->FillRectangle(this->GetRectForCell(x, y), D2Pool::GetSolidColorBrush(color));
		}
	}

	// draw hover
	target->FillRectangle(this->GetRectForCell(mMouseTileX, mMouseTileY), D2Pool::GetSolidColorBrush(D2D1::ColorF::Black, 0.2f));

	for (int x = -gridOffsetX; x <= mWidth; x += mTileSize) {
		target->DrawLine(D2D1::Point2F(x, 0), D2D1::Point2F(x, mHeight), D2Pool::GetSolidColorBrush(D2D1::ColorF(0x005000)));
	}
	for (int y = -gridOffsetY; y <= mHeight; y += mTileSize) {
		target->DrawLine(D2D1::Point2F(0, y), D2D1::Point2F(mWidth, y), D2Pool::GetSolidColorBrush(D2D1::ColorF(0x005000)));
	}

	target->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);

	// draw player
	if (mPlayer) {
		DrawUnit(target, mPlayer);
	}

	// debug
	auto text_aa_old = target->GetTextAntialiasMode();
	target->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_ALIASED);

	D2Pool::PrintText(D2Pool::FormatString(L"Mouse @(%n | %n)", mMouseTileX, mMouseTileY), target, D2Pool::GetFormat(D2PoolFont::MONOSPACE), this->GetContentRectangle(), D2Pool::GetSolidColorBrush(D2D1::ColorF::White), 16.0f, DWRITE_TEXT_ALIGNMENT_CENTER);
	D2Pool::PrintText(D2Pool::FormatString(L"First: (%n | %n)\nLast: (%n | %n)\n\nPlayer (%n | %n)", first_x, first_y, last_x, last_y, (int)std::floor(mPlayer->GetLocation().x), (int)std::floor(mPlayer->GetLocation().y)), target, D2Pool::GetFormat(D2PoolFont::MONOSPACE), this->GetContentRectangle(), D2Pool::GetSolidColorBrush(D2D1::ColorF::White));

	target->SetTextAntialiasMode(text_aa_old);

#if 0
	std::wstring player_items_str = L"";
	mPlayer->GetInventory().ForEachItem([&](ItemToken token, int amount) {
		Item *item = ItemPool::Get(token);
		player_items_str += item->GetName();
		player_items_str += L": ";
		player_items_str += std::to_wstring(amount);
	});
	D2Pool::PrintText(player_items_str, target, D2Pool::GetFormat(D2PoolFont::MONOSPACE), this->GetContentRectangle(), D2Pool::GetSolidColorBrush(D2D1::ColorF::White), DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT_FAR);
#endif
	
	//target->DrawBitmap(D2Pool::GetGraphic(D2Graphic::APPLE), this->GetContentRectangle());

	return true;
}

void MapLayer::DrawUnit(ID2D1RenderTarget *target, Unit *unit)
{
	if (!unit)
		return;

	std::wstring name = unit->GetName();
	auto color = unit->GetColor();

#define UNIT_OVERSIZE 3
	UnitLocation loc = unit->GetLocation();
	float rect_size = mTileSize - 1 + 2 * UNIT_OVERSIZE;
	auto rect = D2D1::RectF(mDragScroll.GetViewX(loc.x * (float)mTileSize) - mTileSize / 2 - UNIT_OVERSIZE, mDragScroll.GetViewY(loc.y * (float)mTileSize) - mTileSize / 2 - UNIT_OVERSIZE);
	rect.right = rect.left + rect_size;
	rect.bottom = rect.top + rect_size;
	auto rrect_face = D2D1::RoundedRect(rect, 3.0f, 3.0f);
	target->FillRoundedRectangle(rrect_face, D2Pool::GetSolidColorBrush(color));
	target->DrawRoundedRectangle(rrect_face, D2Pool::GetSolidColorBrush(D2D1::ColorF(0x333333)), 1.0f);

	if (!name.empty()) {
		std::wstring str;
		str += name[0];
		D2D1_COLOR_F textColor = D2Pool::GetReadableColor(color);
		D2Pool::PrintText(str, target, D2Pool::GetFormat(D2PoolFont::NORMAL), rect, D2Pool::GetSolidColorBrush(textColor), 24.0f, DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	}

#define LABEL_WIDTH 60
	// draw label
	auto rect_label = D2D1::RectF(rect.left - (LABEL_WIDTH / 2 - (rect.right - rect.left) / 2), rect.bottom + 3, rect.right + (LABEL_WIDTH / 2 - (rect.right - rect.left) / 2), rect.bottom + 20 + 3);
	auto rrect_label = D2D1::RoundedRect(rect_label, 2.0f, 2.0f);
	target->FillRoundedRectangle(rrect_label, D2Pool::GetSolidColorBrush(D2D1::ColorF::Black, 0.8f));
	D2Pool::PrintText(unit->GetName(), target, D2Pool::GetFormat(D2PoolFont::NORMAL), rect_label, D2Pool::GetSolidColorBrush(D2D1::ColorF::White), DWRITE_TEXT_ALIGNMENT_CENTER);
}

void MapLayer::SetTileSize(int value)
{
	mTileSize = max(5, value);
	this->Invalidate(INVALIDATION_ALL);
}

void MapLayer::SetPlayer(Player *player)
{
	mPlayer = player;
	this->Invalidate(INVALIDATION_NOCHILDREN);
}

void MapLayer::CenterUnit(Unit *unit)
{
	auto loc = unit->GetLocation();

	mDragScroll.SetOffset(loc.x * mTileSize - mWidth / 2, loc.y * mTileSize - mHeight / 2);
	this->Invalidate(INVALIDATION_NOCHILDREN);
}

bool MUST_CALL MapLayer::OnMouseMove(int x, int y)
{
	bool r = Layer::OnMouseMove(x, y);
	if (r)
	{
		mDragScroll.MouseMove(x, y);

		int oldX = mMouseTileX;
		int oldY = mMouseTileY;

		mMouseTileX = GetTileForXY(mDragScroll.GetRealX(x));
		mMouseTileY = GetTileForXY(mDragScroll.GetRealY(y));

		auto type = mMapData.GetType(MapCoordPair(mMouseTileX, mMouseTileY));

		bool showInfo = type != MapTileType::None;

		if (showInfo) {
			std::wstring text, title;
			auto baseColor = GetMapTileColor(type);
#define favg(x, y) ((x + y) / 2.0f)
#define favg1(x) favg(1.0f, x)
			auto background = D2D1::ColorF(favg1(baseColor.r), favg1(baseColor.g), favg1(baseColor.b), favg1(baseColor.a));

			GetMapTileInfo(type, title, text);

			title += D2Pool::FormatString(L" (%n | %n)", mMouseTileX, mMouseTileY);

			l_info->SetText(text);
			l_info->SetTitle(title);
			l_info->SetBackground(background);
			l_info->SetPosition(max(0, min(x + 15, this->GetWidth() - l_info->GetWidth())), max(0, min(y + 15, this->GetHeight() - l_info->GetHeight())));
			l_info->Show();
		}
		else {
			l_info->Hide();
		}

		if (oldX != mMouseTileX || oldY != mMouseTileY)
			this->Invalidate(INVALIDATION_ALL);
	}
	return r;
}

bool MUST_CALL MapLayer::OnRMouseDown(int x, int y)
{
	if (Layer::OnMouseMove(x, y)) {
		mDragScroll.StartDrag(x, y);
		this->Invalidate(INVALIDATION_ALL);
		return true;
	}
	return false;
}

bool MUST_CALL MapLayer::OnRMouseUp(int x, int y)
{
	mDragScroll.EndDrag();
	this->Invalidate(INVALIDATION_ALL);
	return true;
}

bool MUST_CALL MapLayer::OnLMouseDown(int x, int y)
{
	if (Layer::OnLMouseDown(x, y)) {
		mPlayer->GetInventory().Add(MyGameItems::APPLE, 20);
		this->Invalidate(INVALIDATION_ALL);
		return true;
	}
	return false;
}

void MapLayer::OnKeyDown(int key)
{
	switch (key) {
	case VK_SPACE:
		//this->CenterUnit(mPlayer);
		break;
	}
}

void MUST_CALL MapLayer::OnMouseLeave()
{
	mDragScroll.EndDrag();
	this->Invalidate(INVALIDATION_NOCHILDREN);
}
