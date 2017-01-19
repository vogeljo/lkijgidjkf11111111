#pragma once
#include "native.h"
#include "Util.h"

#include <string>
#include <map>
#include <cstdarg>

#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

class Drawable;
class Layer;

enum class D2PoolFont {
	NORMAL, MONOSPACE
};

enum class D2Graphic {
	NONE, APPLE, SWORD
};

class D2Pool abstract
{
public:
	static ID2D1Factory* GetFactory();
	static ID2D1HwndRenderTarget* CreateWindowRenderTarget(HWND hWnd);
	static ID2D1HwndRenderTarget* CreateWindowRenderTarget(HWND hWnd, int width, int height);
	static ID2D1BitmapRenderTarget* CreateRenderTarget(int width, int height);
	static void SetSourceRenderTarget(ID2D1RenderTarget* target);
	static ID2D1RenderTarget* GetSourceRenderTarget();
	static ID2D1SolidColorBrush* GetSolidColorBrush(D2D1_COLOR_F& color, float opacity = 1.0f);
	static ID2D1SolidColorBrush* GetSolidColorBrush(D2D1::ColorF::Enum color, float opacity = 1.0f);

	static IDWriteFactory* GetWriteFactory();
	static IDWriteTextFormat* GetFormat(D2PoolFont font);

	static void PrintText__OLD(ID2D1RenderTarget* target, IDWriteTextFormat *format, D2D1_RECT_F& rect, ID2D1Brush *brush, std::wstring str, ...);
	static void PrintText(std::wstring str, ID2D1RenderTarget *target, IDWriteTextFormat *format, D2D1_RECT_F& rect, ID2D1Brush *brush);
	static void PrintText(std::wstring str, ID2D1RenderTarget *target, IDWriteTextFormat *format, D2D1_RECT_F& rect, ID2D1Brush *brush, DWRITE_TEXT_ALIGNMENT halign, DWRITE_PARAGRAPH_ALIGNMENT valign = DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
	static void PrintText(std::wstring str, ID2D1RenderTarget *target, IDWriteTextFormat *format, D2D1_RECT_F& rect, ID2D1Brush *brush, float fontSize, DWRITE_TEXT_ALIGNMENT halign, DWRITE_PARAGRAPH_ALIGNMENT valign = DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
	static void PrintText(std::wstring str, ID2D1RenderTarget *target, IDWriteTextFormat *format, D2D1_RECT_F& rect, ID2D1Brush *brush, DWRITE_FONT_WEIGHT weight, DWRITE_TEXT_ALIGNMENT halign, DWRITE_PARAGRAPH_ALIGNMENT valign = DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
	static void PrintText(std::wstring str, ID2D1RenderTarget *target, IDWriteTextFormat *format, D2D1_RECT_F& rect, ID2D1Brush *brush, float fontSize, DWRITE_FONT_WEIGHT weight, DWRITE_TEXT_ALIGNMENT halign, DWRITE_PARAGRAPH_ALIGNMENT valign = DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

	static D2D1_SIZE_F MeasureString(std::wstring str, IDWriteTextFormat *format, D2D1_RECT_F& rect, float fontSize, DWRITE_FONT_WEIGHT weight, DWRITE_TEXT_ALIGNMENT halign, DWRITE_PARAGRAPH_ALIGNMENT valign = DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

	static std::wstring FormatString(std::wstring str, ...);

	static std::wstring IntToMoney(int money, bool currency = true);
	static std::wstring IntToMoneyChange(int money, bool currency = true);

	static D2D1_COLOR_F GetReadableColor(D2D1_COLOR_F& backgroundColor);

	static IWICImagingFactory* GetWICFactory();

	static ID2D1Bitmap* LoadBitmapFromFile(std::wstring file);
	static ID2D1Bitmap* LoadBitmapFromFile(std::wstring file, int width, int height);

	static ID2D1Bitmap* GetGraphic(D2Graphic graphic);

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