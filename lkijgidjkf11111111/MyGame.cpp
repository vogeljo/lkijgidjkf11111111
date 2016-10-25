#include "MyGame.h"
#include "D2Pool.h"

MyGame::MyGame(int width, int height)
	: Game(width, height), nUpdate(1), map_ptr(nullptr)
{
	//map_data.SetType(MapCoordPair(10, 10), MapTileType::Water);
	bool success = MapData::FromFile("default.lkmap", &map_ptr);
	if (!success) {
		map_ptr = new MapData();
		map_ptr->SetType(MapCoordPair(0, 0), MapTileType::Water);
	}

	wchar_t buffer[100] = { 0 };

	GetEnvironmentVariableW(L"USERNAME", buffer, 100);
	uPlayer.SetName(buffer);
	uPlayer.SetColor(D2D1::ColorF(D2D1::ColorF::Orange));
}

void MyGame::Initialize()
{
	MyGameItems::Initialize();

	//uPlayer.SetName(L"Spieler");
	uPlayer.GetStats().Set(Stat::Money, 12000);
	uPlayer.SetLocation(5.0f, 5.0f);

	l_map = new MapLayer(*this, mWidth, mHeight, *map_ptr);
	l_map->SetPosition(0, 0);
	l_map->SetPlayer(&uPlayer);

	l_cash = new CashDisplayLayer(*this, uPlayer, 250, 40);
	l_cash->SetPosition(this->GetWidth() - l_cash->GetWidth(), this->GetHeight() - l_cash->GetHeight());

	l_inventory = new InventoryLayer(*this, mWidth, mHeight, uPlayer.GetInventory());
	l_inventory->SetPosition(mWidth / 2 - l_inventory->GetWidth() / 2, mHeight / 2 - l_inventory->GetHeight() / 2);
	l_inventory->SetVisible(false);

	this->AddLayer(l_map);
	this->AddLayer(l_inventory);
	this->AddLayer(l_cash);

	this->SetFocus(l_map);
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
		if (l_inventory->IsVisible())
			l_inventory->Hide();
		else
			this->OpenInventory();
		break;
	default:
		break;
	}
	Game::OnKeyDown(key);
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
	l_inventory->Show();
}

void MyGame::OnUpdate()
{

}

bool MyGame::OnDraw(ID2D1RenderTarget* target)
{
	target->Clear(D2D1::ColorF(D2D1::ColorF::CornflowerBlue));

	return true;
}