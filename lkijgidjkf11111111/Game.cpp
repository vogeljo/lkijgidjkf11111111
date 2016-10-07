#include "Game.h"
#include <windowsx.h>

LRESULT WINAPI GameWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	CGame *game = (CGame*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

	int mouse_x, mouse_y;

	mouse_x = GET_X_LPARAM(lParam);
	mouse_y = GET_Y_LPARAM(lParam);

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
		game->OnMouseMove(mouse_x, mouse_y);
		break;
	case WM_LBUTTONDOWN:
		game->OnLMouseDown(mouse_x, mouse_y);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

CGame::CGame(int width, int height)
	: Layer(nullptr)
{

#define MY_INSTANCE GetModuleHandle(0)
#define MY_WINDOW_CLASS TEXT("lkijgidjkfWindow")
#define MY_WINDOW_STYLE (WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME))

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

	mWindow = CreateWindow(MY_WINDOW_CLASS, TEXT("My Window!"), MY_WINDOW_STYLE, CW_USEDEFAULT, CW_USEDEFAULT, r.right - r.left, r.bottom - r.top, NULL, NULL, MY_INSTANCE, this);

	ID2D1RenderTarget *rt = D2Pool::CreateWindowRenderTarget(mWindow);

	this->SetTarget(rt);

	D2Pool::SetSourceRenderTarget(this->GetTarget());
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
