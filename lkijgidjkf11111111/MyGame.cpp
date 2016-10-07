#include "MyGame.h"
#include "D2Pool.h"

MyGame::MyGame(int width, int height)
	: CGame(width, height), nUpdate(1)
{

}

void MyGame::Initialize()
{
	cl = new ProgressBarLayer(1024, 2);
	cl->SetPosition(0, 768 - 2);
	this->AddLayer(cl);
}

bool MUST_CALL MyGame::OnMouseMove(int x, int y)
{
	return CGame::OnMouseMove(x, y);
	//if (cl)
	//	cl->SetPosition(x, y);
}

MyGame::~MyGame()
{
}

void MyGame::OnUpdate()
{
	//cl->SetPosition(this->GetMousePosX(), this->GetMousePosY());
	//if (GetKeyState(VK_HOME) & 0x8000)
	//	cl->SetPosition((cl->GetPositionX() + 5) % this->GetWidth(), cl->GetPositionY());
}

bool MyGame::OnDraw(ID2D1RenderTarget* target)
{
	target->Clear(D2D1::ColorF(D2D1::ColorF::CornflowerBlue));
	target->FillRectangle(D2D1::RectF(0, 0, 200, 200), D2Pool::GetSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Navy)));

	//target->DrawTextA(L"Hello friends", 13, D2Pool::GetFormat(D2PoolFont::NORMAL), D2D1::RectF(0, 0, 200, 200), D2Pool::GetSolidColorBrush(D2D1::ColorF::White));
	D2Pool::PrintText(target, D2Pool::GetFormat(D2PoolFont::NORMAL), D2D1::RectF(5, 5, 200, 200), D2Pool::GetSolidColorBrush(D2D1::ColorF::White), L"My String #%n!!!", nUpdate);

	++nUpdate;

	return true;
}