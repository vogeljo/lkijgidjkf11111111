#include "Game.h"
#include <windowsx.h>

LRESULT WINAPI GameWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	Game *game = (Game*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

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
	case WM_KEYDOWN:
		game->OnKeyDown(wParam);
		if (wParam == VK_ESCAPE)
			game->Exit();
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

HWND CreateGameWindow(Game *game, int width, int height) {
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

Game::Game(int width, int height)
	: Layer((mWindowTarget = D2Pool::CreateWindowRenderTarget(mWindow = CreateGameWindow(this, width, height), width, height), width), height)
{

}

Game::~Game()
{
}

HWND Game::GetWindowHandle()
{
	return mWindow;
}

void Game::BindWindow(HWND handle)
{
	//SetWindowLongPtr(handle, GWLP_USERDATA, (LONG_PTR)this);
	//SetWindowLongPtr(handle, GWLP_WNDPROC, (LONG_PTR)GameWndProc);
}

int Game::GetDisplayWidth()
{
	RECT r;
	GetClientRect(mWindow, &r);
	return r.right - r.left;
}


int Game::GetDisplayHeight()
{
	RECT r;
	GetClientRect(mWindow, &r);
	return r.bottom - r.top;
}

int Game::GetMousePosX()
{
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(this->GetWindowHandle(), &p);
	return p.x;
}

int Game::GetMousePosY()
{
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(this->GetWindowHandle(), &p);
	return p.y;
}

void Game::SetFocus(Layer *layer)
{
	mFocused = layer;
}

void Game::Show()
{
	ShowWindow(mWindow, SW_SHOW);
}

void Game::SetFullscreen()
{
	auto val = GetWindowLongPtr(mWindow, GWL_STYLE);
	val &= ~(WS_OVERLAPPEDWINDOW);
	SetWindowLongPtr(mWindow, GWL_STYLE, val);
	SetWindowPos(mWindow, NULL, 0, 0, Util::GetScreenWidth(), Util::GetScreenHeight(), SWP_NOMOVE | SWP_NOZORDER);

	/*WINDOWPLACEMENT wp;
	wp.flags = WPF_RESTORETOMAXIMIZED;
	SetWindowPlacement(mWindow, &wp);*/
}

void Game::Exit()
{
	DestroyWindow(mWindow);
}

void Game::Draw(ID2D1RenderTarget* target)
{
	int tw = target->GetSize().width;
	int th = target->GetSize().height;

	mWindowTarget->BeginDraw();
	
	Layer::Draw(target);
	
	ID2D1Bitmap *bmp = nullptr;
	this->GetTarget()->GetBitmap(&bmp);
	int w = bmp->GetSize().width;
	int h = bmp->GetSize().height;
	mWindowTarget->DrawBitmap(bmp, this->GetBoundingRectangle());

	mWindowTarget->EndDraw();
}

void Game::OnUpdate()
{
	//throw std::logic_error("The method or operation is not implemented.");
}

bool Game::OnDraw(ID2D1RenderTarget* target)
{
	//throw std::logic_error("The method or operation is not implemented.");
	return true;
}

void Game::OnKeyDown(int key)
{
	if (mFocused) {
		mFocused->OnKeyDown(key);
	}
}

bool MUST_CALL Game::OnMouseMove(int x, int y)
{
	auto l = this->FindLayerAt(x, y);

	if (l != mMouseHover) {
		if (mMouseHover)
			mMouseHover->OnMouseLeave();
		if (l != this)
			l->OnMouseEnter();
	}

	mMouseHover = (l == this) ? nullptr : l;

	return Layer::OnMouseMove(x, y);
}
