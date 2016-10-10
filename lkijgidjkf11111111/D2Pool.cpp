#include "D2Pool.h"

#include "Drawable.h"
#include "Layer.h"

#include <map>
#include <cstdarg>

static ID2D1RenderTarget* source_rt = nullptr;

ID2D1Factory* D2Pool::GetFactory()
{
	static ID2D1Factory* factory = nullptr;
	if (!factory)
		D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
	return factory;
}

ID2D1HwndRenderTarget* D2Pool::CreateWindowRenderTarget(HWND hWnd)
{
	ID2D1HwndRenderTarget* target = nullptr;

	RECT r;
	GetClientRect(hWnd, &r);
	//AdjustWindowRect(&r, GetWindowLongPtr(hWnd, GWL_STYLE), FALSE);
	D2D1_SIZE_U size = D2D1::SizeU(r.right - r.left, r.bottom - r.top);

	GetFactory()->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(hWnd, size), &target);
	return target;
}

ID2D1BitmapRenderTarget* D2Pool::CreateRenderTarget(int width, int height)
{
	ID2D1BitmapRenderTarget* target = nullptr;
	GetSourceRenderTarget()->CreateCompatibleRenderTarget(D2D1::SizeF(width, height), &target);
	return target;
}

void D2Pool::SetSourceRenderTarget(ID2D1RenderTarget* target)
{
	source_rt = target;
}

ID2D1RenderTarget* D2Pool::GetSourceRenderTarget()
{
	return source_rt;
}

ID2D1SolidColorBrush* D2Pool::GetSolidColorBrush(D2D1_COLOR_F& color, float opacity /*= 1.0f*/)
{
	static ID2D1SolidColorBrush* brush = nullptr;
	if (!brush) {
		if (!GetSourceRenderTarget())
			return nullptr;
		GetSourceRenderTarget()->CreateSolidColorBrush(color, &brush);
	}

	brush->SetColor(color);
	brush->SetOpacity(opacity);

	return brush;
}

ID2D1SolidColorBrush* D2Pool::GetSolidColorBrush(D2D1::ColorF::Enum color, float opacity /*= 1.0f*/)
{
	return GetSolidColorBrush(D2D1::ColorF(color), opacity);
}

IDWriteFactory* D2Pool::GetWriteFactory()
{
	IDWriteFactory *factory = nullptr;
	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)&factory);
	return factory;
}

IDWriteTextFormat* D2Pool::GetFormat(D2PoolFont font)
{
	static std::map<D2PoolFont, IDWriteTextFormat*> formats;

	auto f = formats.find(font);
	if (f != formats.end())
		return f->second;
	else {
		IDWriteTextFormat *format = nullptr;
		IDWriteFactory *factory = GetWriteFactory();
		switch (font) {
		case D2PoolFont::NORMAL:
			factory->CreateTextFormat(L"Arial", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 12.0f, L"de-de", &format);
			break;
		}
		return formats[font] = format;
	}
}

void D2Pool::PrintText__OLD(ID2D1RenderTarget* target, IDWriteTextFormat *format, D2D1_RECT_F& rect, ID2D1Brush *brush, std::wstring str, ...)
{
	va_list argp;
	va_start(argp, str);

	std::wstring rstr = FormatString(str, argp);

	target->DrawTextA(rstr.c_str(), rstr.length(), format, rect, brush);

	va_end(argp);
}

void D2Pool::PrintText(std::wstring str, ID2D1RenderTarget *target, IDWriteTextFormat *format, D2D1_RECT_F& rect, ID2D1Brush *brush)
{
	target->DrawText(str.c_str(), str.length(), format, rect, brush);
}

void D2Pool::PrintText(std::wstring str, ID2D1RenderTarget *target, IDWriteTextFormat *format, D2D1_RECT_F& rect, ID2D1Brush *brush, DWRITE_TEXT_ALIGNMENT halign, DWRITE_PARAGRAPH_ALIGNMENT valign)
{
	IDWriteTextLayout *layout;
	GetWriteFactory()->CreateTextLayout(str.c_str(), str.length(), format, rect.right - rect.left, rect.bottom - rect.top, &layout);
	layout->SetTextAlignment(halign);
	layout->SetParagraphAlignment(valign);
	target->DrawTextLayout(D2D1::Point2F(rect.left, rect.top), layout, brush);
	SafeRelease(&layout);
}

std::wstring D2Pool::FormatString(std::wstring str, ...)
{
	va_list argp;
	va_start(argp, str);

	std::wstring rstr;
	bool is_format = false;
	for each(wchar_t c in str) {
		if (is_format) {
			switch (c) {
			case L'n':
				rstr += std::to_wstring(va_arg(argp, int));
				break;
			case L'%':
			default:
				rstr += c;
				break;
			}
			is_format = false;
		}
		else {
			switch (c) {
			case L'%':
				is_format = true;
				break;
			default:
				rstr += c;
				break;
			}
		}
	}
	va_end(argp);
	return rstr;
}

int D2Pool::RunPipeline(Drawable *drawable)
{
	MSG msg;
	while (true) {
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE)) {
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			drawable->Update();
			drawable->Draw(drawable->GetTarget());
		}
	}
	return msg.wParam;
}
