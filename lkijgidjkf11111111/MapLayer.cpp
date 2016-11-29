#include "MapLayer.h"
#include "MyGame.h"

#define FOG_OPACITY 0.2f

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
  
 
MapLayer::MapLayer(Game& game, int width, int height, MapData& mapData)
	: Layer(game, width, height), mMouseTileX(0), mMouseTileY(0), mDragScroll(width, height), mMapData(mapData), mPlayer(nullptr), testDynamic(0.0f, 1.0f), mVisionRingAlpha(0.0f, 0.5f), mTileSize(32, width / 100, width / 20)
{
	this->SetTileSize(32);
	this->SetPadding(5.0f);

	mDragScroll.SetDragAreaSize(this->mTileSize * mapData.GetWidth() - 1, this->mTileSize * mapData.GetHeight() - 1);

	auto props = D2D1::RadialGradientBrushProperties(D2D1::Point2F(), D2D1::Point2F(), 100, 100);

	D2D1_GRADIENT_STOP stops[] = {
		D2D1::GradientStop(0.7f, D2D1::ColorF(D2D1::ColorF::Black, 0.0f)),
		D2D1::GradientStop(1.0f, D2D1::ColorF(D2D1::ColorF::Black, FOG_OPACITY))
	};
	ID2D1GradientStopCollection *coll = nullptr;
	D2Pool::GetSourceRenderTarget()->CreateGradientStopCollection(stops, 2, D2D1_GAMMA_1_0, D2D1_EXTEND_MODE::D2D1_EXTEND_MODE_CLAMP, &coll);

	D2Pool::GetSourceRenderTarget()->CreateRadialGradientBrush(props, coll, &mBrushVision);
}

MapLayer::~MapLayer()
{
}

void MapLayer::Initialize()
{
	l_info = new InfoLayer(this->GetGame(), 300, 100);
	this->AddLayer(l_info);
	srand(GetTickCount());

	npcThief = new ThiefNPCUnit();
	npcThief->SetName(L"Thief 1");
	npcThief->SetLocation(15.0f, 7.0f);
	npcThief->GetPatrol()->AddWaypoint(10.0f, 10.0f);
}

void MapLayer::OnUpdate()
{
	++nUpdate;

	float speed = 0.2f;

	if (Util::IsKeyDown('W')) {
		mPlayer->SetLocation(mPlayer->GetLocation().x, mPlayer->GetLocation().y - speed);
	}
	if (Util::IsKeyDown('A')) {
		mPlayer->SetLocation(mPlayer->GetLocation().x - speed, mPlayer->GetLocation().y);
	}
	if (Util::IsKeyDown('S')) {
		mPlayer->SetLocation(mPlayer->GetLocation().x, mPlayer->GetLocation().y + speed);
	}
	if (Util::IsKeyDown('D')) {
		mPlayer->SetLocation(mPlayer->GetLocation().x + speed, mPlayer->GetLocation().y);
	}
	if (Util::IsKeyDown(VK_OEM_PLUS)) {
		this->SetTileSize(mTileSize + 1);
	}
	if (Util::IsKeyDown(VK_OEM_MINUS)) {
		this->SetTileSize(mTileSize - 1);
	}

	mPlayer->Update();
	npcThief->Update();

	this->CenterUnit(mPlayer);
}

bool MapLayer::OnDraw(ID2D1RenderTarget* target)
{
	target->Clear(D2D1::ColorF(0x338833));

	int off_x = mDragScroll.GetOffsetX();
	int off_y = mDragScroll.GetOffsetY();

	int gridOffsetX = off_x % mTileSize;
	int gridOffsetY = off_y % mTileSize;

	int first_x = mDragScroll.GetOffsetX() / mTileSize;
	int first_y = mDragScroll.GetOffsetY() / mTileSize;

	int last_x = first_x + (this->GetWidth() / mTileSize);
	int last_y = first_y + (this->GetHeight() / mTileSize);

	std::set<HouseInfo*> houses;

	// draw tiles
	for (int x = first_x; x <= last_x+1; ++x) {
		for (int y = first_y; y <= last_y+1; ++y) {
			UnitLocation loc(x, y);
			bool vision = mPlayer->HasVisionAt(MapCoordPair(x, y));
			auto real_type = mMapData.GetType(MapCoordPair(x, y));
			auto type = vision ? real_type : MapTileType::Unknown;
			auto color = GetMapTileColor(type);
			auto rect = this->GetRectForCell(x, y);
			auto alpha = mPlayer->GetVisionIntensityAt(loc) * 1.5f;
			ID2D1Brush *brush = D2Pool::GetSolidColorBrush(color);

			if (type == MapTileType::House) {
				HouseInfo *hi = (HouseInfo*)mMapData.GetData(MapCoordPair(x, y));
				if (hi)
					houses.insert(hi);
			}

			if (vision && type != MapTileType::None && type != MapTileType::Unknown) {
				brush->SetOpacity(alpha);
				target->FillRectangle(rect, brush);
			}
		}
	}

	for each (auto hi in houses) {
		this->DrawHouse(target, hi);
	}

	// draw hover
	//if (mMouseTileX >= 0 && mMouseTileY >= 0)
	//	target->FillRectangle(this->GetRectForCell(mMouseTileX, mMouseTileY), D2Pool::GetSolidColorBrush(D2D1::ColorF::Black, 0.2f));

	for (int x = -gridOffsetX; x <= mWidth; x += mTileSize) {
		target->DrawLine(D2D1::Point2F(x, 0), D2D1::Point2F(x, mHeight), D2Pool::GetSolidColorBrush(D2D1::ColorF(0x005000)), 0.3f * mTileSize / 50.0f);
	}
	for (int y = -gridOffsetY; y <= mHeight; y += mTileSize) {
		target->DrawLine(D2D1::Point2F(0, y), D2D1::Point2F(mWidth, y), D2Pool::GetSolidColorBrush(D2D1::ColorF(0x005000)), 0.3f * mTileSize / 50.0f);
	}

	target->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);

	// draw player
	if (mPlayer) {
		DrawUnit(target, mPlayer, true);
	}

	if (npcThief) {
		//printf("%f ; %f\n", npcThief->GetLocation().x, npcThief->GetLocation().y);
		if (mPlayer->HasVisionAt(*npcThief))
			DrawUnit(target, npcThief, false);
	}
	
	// debug
	auto text_aa_old = target->GetTextAntialiasMode();
	//target->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_ALIASED);

	D2Pool::PrintText(D2Pool::FormatString(L"Mouse @(%n | %n)", mMouseTileX, mMouseTileY), target, D2Pool::GetFormat(D2PoolFont::MONOSPACE), this->GetContentRectangle(), D2Pool::GetSolidColorBrush(D2D1::ColorF::White), 16.0f, DWRITE_TEXT_ALIGNMENT_CENTER);
	D2Pool::PrintText(
		D2Pool::FormatString(
			L"First: (%n | %n)\nLast: (%n | %n)\n\nPlayer (%f | %f)\nThief (%n | %n)\n[TEST] Dynamic: %f",
			first_x, first_y, last_x, last_y, mPlayer->GetLocation().x, mPlayer->GetLocation().y, (int)std::floor(npcThief->GetLocation().x), (int)std::floor(npcThief->GetLocation().y), testDynamic.Get()
		),
		target, D2Pool::GetFormat(D2PoolFont::MONOSPACE), this->GetContentRectangle(), D2Pool::GetSolidColorBrush(D2D1::ColorF::White)
	);

	target->SetTextAntialiasMode(text_aa_old);

	return false;
}

void MapLayer::DrawUnit(ID2D1RenderTarget *target, Unit *unit, bool drawVision)
{
	if (!unit)
		return;

	std::wstring name = unit->GetName();
	float phealth = (float)unit->GetStats().Get(Stat::Health) / UNIT_STAT_MAX;
	auto color = unit->GetColor();
	auto health_color = Util::TweenHealth(phealth);

	UnitLocation loc = unit->GetLocation();
	auto position_screen = D2D1::Point2F(mDragScroll.GetViewX(loc.x * (float)mTileSize) - mTileSize / 2, mDragScroll.GetViewY(loc.y * (float)mTileSize) - mTileSize / 2);
	auto pos_center = D2D1::Point2F(position_screen.x + (float)mTileSize / 2.0f, position_screen.y + (float)mTileSize / 2.0f);

#define UNIT_OVERSIZE 3
	float rect_size = mTileSize - 1 + 2 * UNIT_OVERSIZE;
	auto rect = D2D1::RectF(position_screen.x - UNIT_OVERSIZE, position_screen.y - UNIT_OVERSIZE);
	rect.right = rect.left + rect_size;
	rect.bottom = rect.top + rect_size;

#define UNIT_RADIUS ((float)mTileSize / 2.0f + UNIT_OVERSIZE)
	auto ellipse = D2D1::Ellipse(pos_center, UNIT_RADIUS, UNIT_RADIUS);
	target->FillEllipse(ellipse, D2Pool::GetSolidColorBrush(color));
	target->DrawEllipse(ellipse, D2Pool::GetSolidColorBrush(D2D1::ColorF(0x333333)), 1.0f);


	auto rect_health = D2D1::RectF(rect.left, rect.top - 10.0f, rect.right, rect.top - 10.0f + 4.0f);
	rect_health.right = rect_health.left + phealth * (rect_health.right - rect_health.left);
	target->FillRectangle(rect_health, D2Pool::GetSolidColorBrush(health_color));

	if (!name.empty()) {
		std::wstring str;
		str += name[0];
		D2D1_COLOR_F textColor = D2Pool::GetReadableColor(color);
		D2Pool::PrintText(str, target, D2Pool::GetFormat(D2PoolFont::NORMAL), rect, D2Pool::GetSolidColorBrush(textColor), (rect.bottom - rect.top) * 2.0f / 3.0f, DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

#define LABEL_WIDTH 60
		// draw label
		auto rect_label = D2D1::RectF(rect.left - (LABEL_WIDTH / 2 - (rect.right - rect.left) / 2), rect.bottom + 3, rect.right + (LABEL_WIDTH / 2 - (rect.right - rect.left) / 2), rect.bottom + 20 + 3);
		auto rrect_label = D2D1::RoundedRect(rect_label, 2.0f, 2.0f);
		target->FillRoundedRectangle(rrect_label, D2Pool::GetSolidColorBrush(D2D1::ColorF::Black, 0.8f));
		D2Pool::PrintText(unit->GetName(), target, D2Pool::GetFormat(D2PoolFont::NORMAL), rect_label, D2Pool::GetSolidColorBrush(D2D1::ColorF::White), DWRITE_TEXT_ALIGNMENT_CENTER);
	}

	if (drawVision) {
		auto radius = unit->GetVisionRange() * mTileSize;
		auto vision_center = pos_center;
		auto vision_rect = D2D1::RectF(std::round(vision_center.x - radius), std::round(vision_center.y - radius));
		vision_rect.right = vision_rect.left + 2 * radius;
		vision_rect.bottom = vision_rect.top + 2 * radius;

		mBrushVision->SetRadiusX(radius);
		mBrushVision->SetRadiusY(radius);
		mBrushVision->SetCenter(vision_center);
		target->FillRectangle(vision_rect, mBrushVision);
		
		//fill remaining space
		//TODO: make this smarter

		// right
		if (vision_rect.right < this->GetWidth()) {
			target->FillRectangle(D2D1::RectF(vision_rect.right, 0.0f, this->GetWidth(), this->GetHeight()), D2Pool::GetSolidColorBrush(D2D1::ColorF::Black, FOG_OPACITY));
		}
		// bottom
		if (vision_rect.bottom < this->GetHeight()) {
			target->FillRectangle(D2D1::RectF(vision_rect.left, vision_rect.bottom, vision_rect.right, this->GetHeight()), D2Pool::GetSolidColorBrush(D2D1::ColorF::Black, FOG_OPACITY));
		}
		// left
		if (vision_rect.left > 0) {
			target->FillRectangle(D2D1::RectF(0.0f, 0.0f, vision_rect.left, this->GetHeight()), D2Pool::GetSolidColorBrush(D2D1::ColorF::Black, FOG_OPACITY));
		}
		// top
		if (vision_rect.top > 0) {
			target->FillRectangle(D2D1::RectF(vision_rect.left, 0.0f, vision_rect.right, vision_rect.top), D2Pool::GetSolidColorBrush(D2D1::ColorF::Black, FOG_OPACITY));
		}

		target->DrawEllipse(D2D1::Ellipse(vision_center, radius, radius), D2Pool::GetSolidColorBrush(D2D1::ColorF::White), std::abs(mVisionRingAlpha.Get()) * 0.2f);
	}
}

void MapLayer::DrawHouse(ID2D1RenderTarget *target, HouseInfo *house)
{
#define DECO_OVERSIZE (mTileSize / 2.0f)

	if (house && house->IsEmpty())
		return;

	auto tl = house->GetTopLeftCorner(),
		br = house->GetBottomRightCorner();
	auto r_tl = this->GetRectForCell(tl.x, tl.y);
	auto r_br = this->GetRectForCell(br.x, br.y);
	auto house_rect = D2D1::RectF(r_tl.left, r_tl.top, r_br.right, r_br.bottom);
		
	if (house->Intersects(*mPlayer)) {
		//target->FillRectangle(house_rect, D2Pool::GetSolidColorBrush(D2D1::ColorF(0xFFFFAA), 0.2f));

		// DECORATION

		ID2D1PathGeometry *geometry = nullptr;
		ID2D1GeometrySink *sink = nullptr;

		D2Pool::GetFactory()->CreatePathGeometry(&geometry);
		geometry->Open(&sink);


		auto p_tl = D2D1::Point2F(r_tl.left - DECO_OVERSIZE, r_tl.top - DECO_OVERSIZE);
		auto p_br = D2D1::Point2F(r_br.right + DECO_OVERSIZE, r_br.bottom + DECO_OVERSIZE);
		auto p_tr = D2D1::Point2F(r_br.right + DECO_OVERSIZE, r_tl.top - DECO_OVERSIZE);
		auto p_bl = D2D1::Point2F(r_tl.left - DECO_OVERSIZE, r_br.bottom + DECO_OVERSIZE);

		float arc_width = p_tr.x - p_tl.x;
		float arc_height = p_bl.y - p_tl.y;

		float bow_radius = arc_width / 2.0f;

		sink->BeginFigure(p_tl, D2D1_FIGURE_BEGIN_FILLED);
		sink->AddArc(D2D1::ArcSegment(p_tr, D2D1::SizeF(bow_radius, bow_radius / 8), 0.0f, D2D1_SWEEP_DIRECTION_CLOCKWISE, D2D1_ARC_SIZE_SMALL));
		sink->AddLine(p_br);
		sink->AddArc(D2D1::ArcSegment(p_bl, D2D1::SizeF(bow_radius, bow_radius / 8), 0.0f, D2D1_SWEEP_DIRECTION_CLOCKWISE, D2D1_ARC_SIZE_SMALL));
		sink->AddLine(p_tl);

		sink->EndFigure(D2D1_FIGURE_END_OPEN);

		sink->Close();

		target->FillGeometry(geometry, D2Pool::GetSolidColorBrush(D2D1::ColorF::White, 0.2f));
		target->DrawGeometry(geometry, D2Pool::GetSolidColorBrush(D2D1::ColorF::White, std::abs(mVisionRingAlpha.Get())));

#define ARC_SIZE (bow_radius / 8.0f + 10.0f)
#define BTN_RECT_SIZE 40.0f

		auto x_mid = p_tl.x + (p_tr.x - p_tl.x) / 2;

		auto btn_rect = D2D1::RectF(x_mid - BTN_RECT_SIZE / 2, p_tl.y - ARC_SIZE - BTN_RECT_SIZE, x_mid + BTN_RECT_SIZE / 2, p_tr.y - ARC_SIZE);

		target->FillRectangle(btn_rect, D2Pool::GetSolidColorBrush(D2D1::ColorF::Black, 0.5f));

		D2Pool::PrintText(L"E", target, D2Pool::GetFormat(D2PoolFont::NORMAL), btn_rect, D2Pool::GetSolidColorBrush(D2D1::ColorF::White), 18.0f, DWRITE_FONT_WEIGHT_BOLD, DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

		target->DrawRectangle(btn_rect, D2Pool::GetSolidColorBrush(D2D1::ColorF::White));

		// END DECORATION
	}
	else {
#define OUTLINE_SIZE DECO_OVERSIZE
		auto outline_rect = D2D1::RectF(house_rect.left - OUTLINE_SIZE, house_rect.top - OUTLINE_SIZE, house_rect.right + OUTLINE_SIZE, house_rect.bottom + OUTLINE_SIZE);
		target->FillRoundedRectangle(D2D1::RoundedRect(outline_rect, mTileSize / M_PI, mTileSize / M_PI), D2Pool::GetSolidColorBrush(D2D1::ColorF::White, 0.1f));
		target->DrawRoundedRectangle(D2D1::RoundedRect(outline_rect, mTileSize / M_PI, mTileSize / M_PI), D2Pool::GetSolidColorBrush(D2D1::ColorF::White, 0.8f), 0.5f * (mTileSize / 32.0f));
	}

	std::vector<std::tuple<D2D1_POINT_2F, D2D1_POINT_2F>> lines;
	float line_width = house->Intersects(*mPlayer) ? 1.0f : 0.5f;
	float outline_size = line_width * (mTileSize / 32.0f);
	float outline_size2 = outline_size / 2.0f;

	for (auto x = tl.x; x <= br.x; ++x) {
		for (auto y = tl.y; y <= br.y; ++y) {

			if (!house->Intersects(x, y))
				continue;

			auto r_cell = this->GetRectForCell(x, y);

			bool has_top = house->Intersects(x, y - 1);
			bool has_bottom = house->Intersects(x, y + 1);
			bool has_left = house->Intersects(x - 1, y);
			bool has_right = house->Intersects(x + 1, y);

			if (!has_top) {
				auto t = std::make_tuple<D2D1_POINT_2F, D2D1_POINT_2F>(
					D2D1::Point2F(r_cell.left, r_cell.top),
					D2D1::Point2F(r_cell.right, r_cell.top)
					);
				lines.push_back(t);
			}
			if (!has_left) {
				auto t = std::make_tuple<D2D1_POINT_2F, D2D1_POINT_2F>(
					D2D1::Point2F(r_cell.left, r_cell.top),
					D2D1::Point2F(r_cell.left, r_cell.bottom)
					);
				lines.push_back(t);
			}
			if (!has_right) {
				auto t = std::make_tuple<D2D1_POINT_2F, D2D1_POINT_2F>(
					D2D1::Point2F(r_cell.right, r_cell.top),
					D2D1::Point2F(r_cell.right, r_cell.bottom)
					);
				lines.push_back(t);
			}
			if (!has_bottom) {
				auto t = std::make_tuple<D2D1_POINT_2F, D2D1_POINT_2F>(
					D2D1::Point2F(r_cell.left, r_cell.bottom),
					D2D1::Point2F(r_cell.right, r_cell.bottom)
					);
				lines.push_back(t);
			}
		}
	}

	D2D1::ColorF line_color = D2D1::ColorF::Black;

	for each (auto t in lines) {
		target->DrawLine(std::get<0>(t), std::get<1>(t), D2Pool::GetSolidColorBrush(line_color), outline_size);
	}

	D2Pool::PrintText(house->GetName(), target, D2Pool::GetFormat(D2PoolFont::NORMAL), house_rect, D2Pool::GetSolidColorBrush(D2D1::ColorF::White, 1.0f), 24.0f, DWRITE_FONT_WEIGHT_BOLD, DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
}

void MapLayer::SetTileSize(int value)
{
	mTileSize = value;
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

		bool showInfo = type != MapTileType::None && mPlayer->HasVisionAt(UnitLocation(mMouseTileX, mMouseTileY));

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
			l_info->SetPosition(std::max(0, std::min(x + 15, this->GetWidth() - l_info->GetWidth())), std::max(0, std::min(y + 15, this->GetHeight() - l_info->GetHeight())));
			l_info->FadeIn(50);
		}
		else if (l_info->IsVisible()) {
			l_info->FadeOut(50);
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
		npcThief->GetPatrol()->AddWaypoint(mPlayer->GetLocation().x, mPlayer->GetLocation().y);
		break;
	case 'P':
		//static_cast<NPCMovementPatrol*>(npcThief->GetMovement())->AddWaypoint(10.0f, 30.0f);
		break;
	}
}

void MUST_CALL MapLayer::OnMouseLeave()
{
	l_info->FadeOut(50);
	mMouseTileX = mMouseTileY = -1;
	mDragScroll.EndDrag();
	this->Invalidate(INVALIDATION_NOCHILDREN);
}
