#include "MyGame.h"

MyGame::MyGame(int width, int height)
	: Game(width, height), nUpdate(1), map_ptr(nullptr)
{
	//map_data.SetType(MapCoordPair(10, 10), MapTileType::Water);
	bool success = MapData::FromFile("resources\\default.lkmap", &map_ptr);
	if (!success) {
		map_ptr = new MapData();
		map_ptr->SetType(MapCoordPair(0, 0), MapTileType::Water);
	}

	wchar_t buffer[100] = { 0 };

	GetEnvironmentVariableW(L"USERNAME", buffer, 100);
	uPlayer.SetName(buffer); 
	uPlayer.SetColor(D2D1::ColorF(D2D1::ColorF::Orange));
	
	HouseInfo *house1 = new HouseInfo(*map_ptr, L"House 1");
	house1->AddField(MapCoordPair(20, 20));
	house1->AddField(MapCoordPair(22, 20));
	house1->AddField(MapCoordPair(21, 21));
	house1->AddField(MapCoordPair(22, 21));
	house1->AddField(MapCoordPair(23, 21));
	house1->AddField(MapCoordPair(20, 22));
	house1->AddField(MapCoordPair(21, 22));
	house1->AddField(MapCoordPair(24, 21));
	house1->AddField(MapCoordPair(25, 21));
	house1->AddField(MapCoordPair(26, 21));
	house1->AddField(MapCoordPair(27, 21));
	house1->AddField(MapCoordPair(28, 21));
	house1->AddField(MapCoordPair(28, 22));
	house1->AddField(MapCoordPair(28, 23));
	house1->AddField(MapCoordPair(28, 22));
	house1->AddField(MapCoordPair(27, 23));
	house1->AddField(MapCoordPair(24, 23));
}

void MyGame::Initialize()
{
	MyGameItems::Initialize();

	//uPlayer.SetName(L"Spieler");
	uPlayer.GetStats().Set(Stat::Money, 12000);
	uPlayer.GetStats().Set(Stat::Health, 1000);
	uPlayer.SetLocation(5.0f, 5.0f);

	l_map = new MapLayer(*this, mWidth, mHeight, *map_ptr);
	l_map->SetPosition(0, 0);
	l_map->SetPlayer(&uPlayer);

	l_cash = new CashDisplayLayer(*this, uPlayer, 300, 40);
	l_cash->SetPosition(this->GetWidth() - l_cash->GetWidth(), this->GetHeight() - l_cash->GetHeight());
	l_cash->SetVisible(false);

	l_inventory = new InventoryLayer(*this, mWidth, mHeight, uPlayer.GetInventory());
	l_inventory->SetPosition(mWidth / 2 - l_inventory->GetWidth() / 2, mHeight / 2 - l_inventory->GetHeight() / 2);
	l_inventory->SetVisible(false);

	l_time = new MyGameTimeLayer(*this, 200, 200, mTime);
	l_time->SetPosition(0, this->GetHeight() - l_time->GetHeight());
	l_time->SetVisible(false);

	l_player_attr = new PlayerAttributesLayer(*this, uPlayer, 300, this->GetHeight() - l_cash->GetHeight());
	l_player_attr->SetPosition(this->GetWidth() - 300, 0);
	l_player_attr->SetVisible(false);

	l_console = new ConsoleLayer(this->GetGame(), this->GetWidth(), 50.0f);
	l_console->SetPosition(0, this->GetHeight() - 50);
	l_console->SetVisible(false);

	this->AddLayer(l_map);

	l_map->AddLayer(l_player_attr);
	l_map->AddLayer(l_cash);
	l_map->AddLayer(l_time);
	l_map->AddLayer(l_inventory);
	l_map->AddLayer(l_console);

	l_map->Show();
}

bool MUST_CALL MyGame::OnMouseMove(int x, int y)
{
	return Game::OnMouseMove(x, y);
}

bool MUST_CALL MyGame::OnLMouseDown(int x, int y)
{
	return Game::OnLMouseDown(x, y);
}

void MyGame::OnKeyDown(int key)
{
	switch (key)
	{
	case VK_TAB:
		if (!l_console->HasFocus()) {
			if (l_inventory->IsVisible())
				l_inventory->FadeOut(100);
			else
				l_inventory->FadeIn(100);
		}
		break;
	case 'C':
		if ((GetKeyState(VK_SHIFT) & 0x8000) && (GetKeyState(VK_CONTROL) & 0x8000)) {
			this->ToggleConsole();
		}
		else if (!l_console->HasFocus()) {
			if (l_player_attr->IsVisible()) {
				l_player_attr->FadeOut(100);
				l_cash->FadeOut(100);
				l_time->FadeOut(100);
			}
			else {
				l_player_attr->FadeIn(100);
				l_cash->FadeIn(100);
				l_time->FadeIn(100);
			}
		}
		break;
	default:
		break;
	}
	Game::OnKeyDown(key);
}

void MyGame::ToggleConsole()
{
	bool hiding = l_console->IsVisible();
	if (hiding) {
		l_console->FadeOut();
		this->SetFocus(l_map);
	}
	else {
		l_console->FadeIn();
		this->SetFocus(l_console);
	}
}

void MyGame::OnExitKey()
{
	this->Exit();
}

bool MyGame::OnExit()
{
	//return MessageBox(this->GetWindowHandle(), "Wirklich schlie�en?", "Schlie�en", MB_YESNO | MB_ICONQUESTION) == IDYES;
	return true;
}

MyGame::~MyGame()
{
	if (map_ptr) {
		delete map_ptr;
		map_ptr = nullptr;
	}
}

void MyGame::OpenInventory()
{
	l_inventory->FadeIn();
}

void MyGame::OnUpdate()
{
	if (Util::IsKeyDown(VK_OEM_COMMA))
		uPlayer.GetStats().Substract(Stat::Health, 1);
	if (Util::IsKeyDown(VK_OEM_PERIOD))
		uPlayer.GetStats().Add(Stat::Health, 1);

	mTime.Update();
}

bool MyGame::OnDraw(ID2D1RenderTarget* target)
{
	target->Clear(D2D1::ColorF(D2D1::ColorF::CornflowerBlue));

	target->SetTransform(D2D1::Matrix3x2F::Rotation(3.0f));

	D2Pool::PrintText(L"lkijgidjkf11111111", target, D2Pool::GetFormat(D2PoolFont::MONOSPACE), this->GetContentRectangle(), D2Pool::GetSolidColorBrush(D2D1::ColorF::Black), 80.0f, DWRITE_FONT_WEIGHT_EXTRA_BLACK, DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	target->SetTransform(D2D1::Matrix3x2F::Identity());

	return true;
}