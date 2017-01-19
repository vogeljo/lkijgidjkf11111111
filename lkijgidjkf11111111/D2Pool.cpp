#include "D2Pool.h"

#include "Drawable.h"
#include "Layer.h"

//
HRESULT RealLoadBitmapFromFile(ID2D1RenderTarget *pRenderTarget, IWICImagingFactory *pIWICFactory, PCWSTR uri,
	UINT destinationWidth, UINT destinationHeight, ID2D1Bitmap **ppBitmap)
{
	IWICBitmapDecoder *pDecoder = NULL;
	IWICBitmapFrameDecode *pSource = NULL;
	IWICStream*pStream = NULL;
	IWICFormatConverter *pConverter = NULL;
	IWICBitmapScaler *pScaler = NULL;

	HRESULT hr = pIWICFactory->CreateDecoderFromFilename(uri, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &pDecoder);

	if (SUCCEEDED(hr)) {
		hr = pDecoder->GetFrame(0, &pSource);
	}

	if (SUCCEEDED(hr)) {
		// Convert Image format to 32bppPBGRA
		// (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
		hr = pIWICFactory->CreateFormatConverter(&pConverter);
	}

	if (SUCCEEDED(hr)) {
		hr = pConverter->Initialize(
			pSource,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			NULL,
			0.f,
			WICBitmapPaletteTypeMedianCut
		);
	}

	if (SUCCEEDED(hr)) {

		// Create a Direct2D bitmap from the WIC bitmap
		hr = pRenderTarget->CreateBitmapFromWicBitmap(
			pConverter,
			NULL,
			ppBitmap
		);
	}

	SafeRelease(&pDecoder);
	SafeRelease(&pSource);
	SafeRelease(&pStream);
	SafeRelease(&pConverter);
	SafeRelease(&pScaler);

	return hr;
}

//

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
	RECT r;
	GetClientRect(hWnd, &r);

	return CreateWindowRenderTarget(hWnd, r.right - r.left, r.bottom - r.top);
}

ID2D1HwndRenderTarget* D2Pool::CreateWindowRenderTarget(HWND hWnd, int width, int height)
{
	ID2D1HwndRenderTarget* target = nullptr;

	D2D1_SIZE_U size = D2D1::SizeU(width, height);

	GetFactory()->CreateHwndRenderTarget(D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED)), D2D1::HwndRenderTargetProperties(hWnd, size), &target);
	D2Pool::SetSourceRenderTarget(target);
	return target;
}

ID2D1BitmapRenderTarget* D2Pool::CreateRenderTarget(int width, int height)
{
	ID2D1BitmapRenderTarget* target = nullptr;
	D2D1_SIZE_F size = D2D1::SizeF(width, height);
	D2D1_PIXEL_FORMAT pformat = D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED);
	GetSourceRenderTarget()->CreateCompatibleRenderTarget(&size, nullptr, &pformat, D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS_NONE, &target);
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
			factory->CreateTextFormat(L"Corbel", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 14.0f, L"de-de", &format);
			break;
		case D2PoolFont::MONOSPACE:
			factory->CreateTextFormat(L"Consolas", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 13.0f, L"de-de", &format);
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

void D2Pool::PrintText(std::wstring str, ID2D1RenderTarget *target, IDWriteTextFormat *format, D2D1_RECT_F& rect, ID2D1Brush *brush, float fontSize, DWRITE_TEXT_ALIGNMENT halign = DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT valign /*= DWRITE_PARAGRAPH_ALIGNMENT_NEAR*/)
{
	IDWriteTextLayout *layout;
	GetWriteFactory()->CreateTextLayout(str.c_str(), str.length(), format, rect.right - rect.left, rect.bottom - rect.top, &layout);
	layout->SetTextAlignment(halign);
	layout->SetParagraphAlignment(valign);
	DWRITE_TEXT_RANGE range;
	range.startPosition = 0;
	range.length = str.length();
	layout->SetFontSize(fontSize, range);
	target->DrawTextLayout(D2D1::Point2F(rect.left, rect.top), layout, brush);
	SafeRelease(&layout);
}

void D2Pool::PrintText(std::wstring str, ID2D1RenderTarget *target, IDWriteTextFormat *format, D2D1_RECT_F& rect, ID2D1Brush *brush, DWRITE_FONT_WEIGHT weight, DWRITE_TEXT_ALIGNMENT halign, DWRITE_PARAGRAPH_ALIGNMENT valign /*= DWRITE_PARAGRAPH_ALIGNMENT_NEAR*/)
{
	IDWriteTextLayout *layout;
	GetWriteFactory()->CreateTextLayout(str.c_str(), str.length(), format, rect.right - rect.left, rect.bottom - rect.top, &layout);
	layout->SetTextAlignment(halign);
	layout->SetParagraphAlignment(valign);
	DWRITE_TEXT_RANGE range;
	range.startPosition = 0;
	range.length = str.length();
	layout->SetFontWeight(weight, range);
	target->DrawTextLayout(D2D1::Point2F(rect.left, rect.top), layout, brush);
	SafeRelease(&layout);
}

void D2Pool::PrintText(std::wstring str, ID2D1RenderTarget *target, IDWriteTextFormat *format, D2D1_RECT_F& rect, ID2D1Brush *brush, float fontSize, DWRITE_FONT_WEIGHT weight, DWRITE_TEXT_ALIGNMENT halign, DWRITE_PARAGRAPH_ALIGNMENT valign /*= DWRITE_PARAGRAPH_ALIGNMENT_NEAR*/)
{
	IDWriteTextLayout *layout;
	GetWriteFactory()->CreateTextLayout(str.c_str(), str.length(), format, rect.right - rect.left, rect.bottom - rect.top, &layout);
	layout->SetTextAlignment(halign);
	layout->SetParagraphAlignment(valign);
	DWRITE_TEXT_RANGE range;
	range.startPosition = 0;
	range.length = str.length();
	layout->SetFontSize(fontSize, range);
	layout->SetFontWeight(weight, range);
	target->DrawTextLayout(D2D1::Point2F(rect.left, rect.top), layout, brush);
	SafeRelease(&layout);
}

D2D1_SIZE_F D2Pool::MeasureString(std::wstring str, IDWriteTextFormat *format, D2D1_RECT_F& rect, float fontSize, DWRITE_FONT_WEIGHT weight, DWRITE_TEXT_ALIGNMENT halign, DWRITE_PARAGRAPH_ALIGNMENT valign /*= DWRITE_PARAGRAPH_ALIGNMENT_NEAR*/)
{
	IDWriteTextLayout *layout;
	GetWriteFactory()->CreateTextLayout(str.c_str(), str.length(), format, rect.right - rect.left, rect.bottom - rect.top, &layout);
	layout->SetTextAlignment(halign);
	layout->SetParagraphAlignment(valign);
	DWRITE_TEXT_RANGE range;
	range.startPosition = 0;
	range.length = str.length();
	layout->SetFontSize(fontSize, range);
	layout->SetFontWeight(weight, range);

	DWRITE_TEXT_METRICS metrics;

	layout->GetMetrics(&metrics);
	SafeRelease(&layout);

	return D2D1::SizeF(metrics.widthIncludingTrailingWhitespace, metrics.height);
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
			case L's':
				rstr += std::wstring(va_arg(argp, wchar_t*));
				break;
			case L'f': {
				rstr += std::to_wstring(va_arg(argp, double));
				break;
			}
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

std::wstring D2Pool::IntToMoney(int money, bool currency /*= true*/)
{
	int cent = abs(money % 100);

	std::wstring ws;
	ws += std::to_wstring(money / 100);
	ws += L',';
	if (cent < 10)
		ws += L'0';
	ws += std::to_wstring(cent);
	if (currency)
		ws += L" €";
	return ws;
}

std::wstring D2Pool::IntToMoneyChange(int money, bool currency /* = true*/)
{
	std::wstring ws;
	wchar_t c = money >= 0 ? L'+' : L'-';

	ws += c;
	ws += IntToMoney(abs(money), currency);
	return ws;
}

D2D1_COLOR_F D2Pool::GetReadableColor(D2D1_COLOR_F& backgroundColor)
{
	// .2126 R, .7152 G, .0722 B
	return D2D1::ColorF(backgroundColor.r * 0.2126 + backgroundColor.g * 0.7152 + backgroundColor.b * 0.0722 > 0.5f ? D2D1::ColorF::Black : D2D1::ColorF::White);
}

IWICImagingFactory* D2Pool::GetWICFactory()
{
	static IWICImagingFactory *factory = nullptr;
	if (!factory) {
		CoInitialize(NULL);
		CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)&factory);
	}
	return factory;
}

ID2D1Bitmap* D2Pool::LoadBitmapFromFile(std::wstring file)
{
	return LoadBitmapFromFile(file, 0, 0);
}

ID2D1Bitmap* D2Pool::LoadBitmapFromFile(std::wstring file, int width, int height)
{
	ID2D1Bitmap *bmp = nullptr;
	RealLoadBitmapFromFile(D2Pool::GetSourceRenderTarget(), D2Pool::GetWICFactory(), file.c_str(), width, height, &bmp);
	return bmp;
}

ID2D1Bitmap* D2Pool::GetGraphic(D2Graphic graphic)
{
	static std::map<D2Graphic, ID2D1Bitmap*> values;
	auto it = values.find(graphic);
	if (it != values.end())
		return it->second;
	else {
		std::wstring filename = L"";
		switch (graphic) {
		case D2Graphic::APPLE:
			filename = L"apple.png";
			break;
		case D2Graphic::SWORD:
			filename = L"sword.png";
			break;
		case D2Graphic::KNIFE:
			filename = L"knife.png";
			break;
		}
		if (filename.empty())
			return nullptr;

		std::wstring foldername = L"resources\\";

		return values[graphic] = D2Pool::LoadBitmapFromFile(foldername + filename);
	}
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
			Util::PushTime();

			drawable->Update();
			drawable->Draw(drawable->GetTarget());

			Util::PopTime();
		}
	}
	return msg.wParam;
}
