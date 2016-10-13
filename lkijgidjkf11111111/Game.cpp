#include "Game.h"
#include <windowsx.h>

LRESULT WINAPI GameWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	CGame *game = (CGame*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

	int mouse_x, mouse_y;

	mouse_x = GET_X_LPARAM(lParam);
	mouse_y = GET_Y_LPARAM(lParam);

	int mouse_xs;
	int mouse_ys;
	if (game) {
		mouse_xs = (float)game->GetWidth() / (float)game->GetDisplayWidth() * mouse_x;
		mouse_ys = (float)game->GetHeight() / (float)game->GetDisplayHeight() * mouse_y;
	}

	CREATESTRUCT *crs;

	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_CREATE:
		crs = (CREATESTRUCT*)lParam;
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)crs->lpCreateParams);
		return 0;
	case WM_MOUSEMOVE:
		game->OnMouseMove(mouse_xs, mouse_ys);
		break;
	case WM_LBUTTONDOWN:
		game->OnLMouseDown(mouse_xs, mouse_ys);
		break;
	case WM_RBUTTONDOWN:
		game->OnRMouseDown(mouse_xs, mouse_ys);
		break;
	case WM_LBUTTONUP:
		game->OnLMouseUp(mouse_xs, mouse_ys);
		break;
	case WM_RBUTTONUP:
		game->OnRMouseUp(mouse_xs, mouse_ys);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

HWND CreateGameWindow(CGame *game, int width, int height) {
#define MY_INSTANCE GetModuleHandle(0)
#define MY_WINDOW_CLASS TEXT("lkijgidjkfWindow")
#define MY_WINDOW_STYLE (WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME) | WS_VISIBLE)

	WNDCLASS cls;
	ZeroMemory(&cls, sizeof(cls));
	cls.hbrBackground = (HBRUSH)COLOR_WINDOW;
	cls.hCursor = LoadCursor(NULL, IDC_ARROW);
	cls.hInstance = MY_INSTANCE;
	cls.lpfnWndProc = GameWndProc;
	cls.lpszClassName = MY_WINDOW_CLASS;
	RegisterClass(&cls);

	RECT r = { 0, 0, width, height };
	AdjustWindowRect(&r, MY_WINDOW_STYLE, FALSE);

	return CreateWindow(MY_WINDOW_CLASS, TEXT("My Window!"), MY_WINDOW_STYLE, CW_USEDEFAULT, CW_USEDEFAULT, r.right - r.left, r.bottom - r.top, NULL, NULL, MY_INSTANCE, game);
}

CGame::CGame(int width, int height)
	: Layer((mWindowTarget = D2Pool::CreateWindowRenderTarget(mWindow = CreateGameWindow(this, width, height)), width), height)
{
	//D2Pool::SetSourceRenderTarget(rt);
}

CGame::~CGame()
{
}

HWND CGame::GetWindowHandle()
{
	return mWindow;
}

void CGame::BindWindow(HWND handle)
{
	//SetWindowLongPtr(handle, GWLP_USERDATA, (LONG_PTR)this);
	//SetWindowLongPtr(handle, GWLP_WNDPROC, (LONG_PTR)GameWndProc);
}

int CGame::GetDisplayWidth()
{
	RECT r;
	GetClientRect(mWindow, &r);
	return r.right - r.left;
}


int CGame::GetDisplayHeight()
{
	RECT r;
	GetClientRect(mWindow, &r);
	return r.bottom - r.top;
}

int CGame::GetMousePosX()
{
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(this->GetWindowHandle(), &p);
	return p.x;
}

int CGame::GetMousePosY()
{
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(this->GetWindowHandle(), &p);
	return p.y;
}

void CGame::Show()
{
	ShowWindow(mWindow, SW_SHOW);
}

void CGame::Draw(ID2D1RenderTarget* target)
{
	Layer::Draw(target);

	mWindowTarget->BeginDraw();

	ID2D1Bitmap *bmp = nullptr;
	this->GetTarget()->GetBitmap(&bmp);
	mWindowTarget->DrawBitmap(bmp, this->GetRectangle());

	mWindowTarget->EndDraw();
}

void CGame::OnUpdate()
{
	//throw std::logic_error("The method or operation is not implemented.");
}

bool CGame::OnDraw(ID2D1RenderTarget* target)
{
	//throw std::logic_error("The method or operation is not implemented.");
	return true;
}
