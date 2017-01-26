#pragma once
#include "Layer.h"
#include "Util.h"
#include "ShellEngine.h"

#include "native.h"


enum class CGameSpecialDimensions {
	FULLSCREEN
};

class Game abstract
	: public Layer
{
protected:
	HWND mWindow;
	ID2D1HwndRenderTarget *mWindowTarget;

	Layer *mFocused;
	Layer *mMouseHover;

	ShellEngine mShell;
public:
	Game(int width, int height);
	virtual ~Game();

	HWND GetWindowHandle();

	void BindWindow(HWND handle);

	int GetDisplayWidth();
	int GetDisplayHeight();

	int GetMousePosX();
	int GetMousePosY();
	void SetFocus(Layer *layer);

	ShellEngine& GetShellEngine();

	void Show();
	void SetFullscreen();

	virtual void Exit();

	virtual void Draw(ID2D1RenderTarget* target) override;

	virtual void OnUpdate() override;
	virtual bool OnDraw(ID2D1RenderTarget* target) override;


	virtual void OnKeyDown(int key) override;

	/*template<class T>
	static T* CreateFullscreen() {
		int screen_w = Util::GetScreenWidth();
		int screen_h = Util::GetScreenHeight();
		T* r = new T(screen_w, screen_h);
		r->MakeBorderless();
		//r->Maximize();
		r->MakeTopMost();
		return r;
	}*/

	virtual bool MUST_CALL OnMouseMove(int x, int y) override;


	virtual void MUST_CALL OnKeyChar(wchar_t c) override;

	Layer* GetFocused();

	bool Shell(const std::wstring& command);
};

