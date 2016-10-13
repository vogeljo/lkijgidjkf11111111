#include "MyGame.h"
#include "D2Pool.h"

MyGame::MyGame(int width, int height)
	: CGame(width, height), nUpdate(1)
{

}

void MyGame::Initialize()
{
	uPlayer.SetName(L"Spieler");
	uPlayer.GetStats().Set(Stat::Money, 12000);

	l_map = new MapLayer(mWidth, mHeight);
	l_map->SetPosition(0, 0);

	l_cash = new CashDisplayLayer(uPlayer, 250, 40);
	l_cash->SetPosition(this->GetWidth() - l_cash->GetWidth(), this->GetHeight() - l_cash->GetHeight());

	//cl = new ProgressBarLayer(this->GetWidth() - l_cash->GetWidth(), 2);
	//cl->SetPosition(0, this->GetHeight() - 2);

	this->AddLayer(l_map);
	this->AddLayer(l_cash);
	//this->AddLayer(cl);
}

bool MUST_CALL MyGame::OnMouseMove(int x, int y)
{
	return CGame::OnMouseMove(x, y);
}

bool MUST_CALL MyGame::OnLMouseDown(int x, int y)
{
	return CGame::OnLMouseDown(x, y);
}

MyGame::~MyGame()
{
}

void MyGame::OnUpdate()
{

}

bool MyGame::OnDraw(ID2D1RenderTarget* target)
{
	target->Clear(D2D1::ColorF(D2D1::ColorF::CornflowerBlue));
	return true;
}