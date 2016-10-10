#include "MyGame.h"
#include "D2Pool.h"

MyGame::MyGame(int width, int height)
	: CGame(width, height), nUpdate(1)
{

}

void MyGame::Initialize()
{
	cl = new ProgressBarLayer(800, 2);
	cl->SetPosition(0, 600 - 2);
	this->AddLayer(cl);

	l_cash = new CashDisplayLayer(100, 200, 100);
	l_cash->SetPosition(300, 5);
	this->AddLayer(l_cash);
}

bool MUST_CALL MyGame::OnMouseMove(int x, int y)
{
	return CGame::OnMouseMove(x, y);
	//if (cl)
	//	cl->SetPosition(x, y);
}

bool MUST_CALL MyGame::OnLMouseDown(int x, int y)
{
	cl->SetVisible(false);
	return CGame::OnLMouseDown(x, y);
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
	//D2Pool::PrintText(target, D2Pool::GetFormat(D2PoolFont::NORMAL), D2D1::RectF(5, 5, 200, 200), D2Pool::GetSolidColorBrush(D2D1::ColorF::White), L"My String #%n!!!", nUpdate);
	//target->DrawText(D2Pool::FormatString(L"My String #%n!!!", nUpdate).c_str(), )
	D2Pool::PrintText(D2Pool::FormatString(L"#%n", nUpdate), target, D2Pool::GetFormat(D2PoolFont::NORMAL), D2D1::RectF(0, 0, 200, 200), D2Pool::GetSolidColorBrush(D2D1::ColorF::White), DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	++nUpdate;

	return true;
}