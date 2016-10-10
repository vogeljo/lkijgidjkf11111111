#pragma once
#include <d2d1.h>
#include <dwrite.h>
#include <string>

class Drawable;
class Layer;

enum class D2PoolFont {
	NORMAL
};

class D2Pool abstract
{
public:
	static ID2D1Factory* GetFactory();
	static ID2D1HwndRenderTarget* CreateWindowRenderTarget(HWND hWnd);
	static ID2D1BitmapRenderTarget* CreateRenderTarget(int width, int height);
	static void SetSourceRenderTarget(ID2D1RenderTarget* target);
	static ID2D1RenderTarget* GetSourceRenderTarget();
	static ID2D1SolidColorBrush* GetSolidColorBrush(D2D1_COLOR_F& color, float opacity = 1.0f);
	static ID2D1SolidColorBrush* GetSolidColorBrush(D2D1::ColorF::Enum color, float opacity = 1.0f);

	static IDWriteFactory* GetWriteFactory();
	static IDWriteTextFormat* GetFormat(D2PoolFont font);

	static void PrintText__OLD(ID2D1RenderTarget* target, IDWriteTextFormat *format, D2D1_RECT_F& rect, ID2D1Brush *brush, std::wstring str, ...);
	static void PrintText(std::wstring str, ID2D1RenderTarget *target, IDWriteTextFormat *format, D2D1_RECT_F& rect, ID2D1Brush *brush);
	static void PrintText(std::wstring str, ID2D1RenderTarget *target, IDWriteTextFormat *format, D2D1_RECT_F& rect, ID2D1Brush *brush, DWRITE_TEXT_ALIGNMENT halign = DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT valign = DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

	static std::wstring FormatString(std::wstring str, ...);

	static int RunPipeline(Drawable *drawable);
};

template<typename T>
void SafeRelease(T** ptr) {
	if (*ptr) {
		(*ptr)->Release();
		*ptr = nullptr;
	}
}

template<Layer* l>
LRESULT WINAPI D2WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}