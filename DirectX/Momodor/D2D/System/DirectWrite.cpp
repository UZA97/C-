#include "framework.h"
#include "DirectWrite.h"

DirectWrite* DirectWrite::instance = NULL;
IDWriteFactory* DirectWrite::writeFactory = NULL;
ID2D1DeviceContext* DirectWrite::deviceContext = NULL;
ID2D1Bitmap1* DirectWrite::bitmap = NULL;
IDXGISurface* DirectWrite::surface = NULL;
ID2D1SolidColorBrush* DirectWrite::brush = NULL;
IDWriteTextFormat* DirectWrite::format = NULL;

void DirectWrite::Create()
{
	assert(instance == NULL);

	instance = new DirectWrite();


}

void DirectWrite::Delete()
{
	SAFE_DELETE(instance);
}

DirectWrite * DirectWrite::Get()
{
	return instance;
}

DirectWrite::DirectWrite()
{
	HRESULT hr;

	hr = DWriteCreateFactory
	(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		(IUnknown**)&writeFactory
	);
	assert(SUCCEEDED(hr));

	D2D1_FACTORY_OPTIONS option;
	option.debugLevel = D2D1_DEBUG_LEVEL_WARNING;
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &factory);
	assert(SUCCEEDED(hr));

	IDXGIDevice* dxgiDevice;
	hr = Device->QueryInterface(&dxgiDevice);
	assert(SUCCEEDED(hr));

	hr = factory->CreateDevice(dxgiDevice, &device);
	assert(SUCCEEDED(hr));

	hr = device->CreateDeviceContext
	(
		D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS,
		&deviceContext
	);
	assert(SUCCEEDED(hr));

	CreateBackBuffer();
}

DirectWrite::~DirectWrite()
{
	DeleteBackBuffer();
}


void DirectWrite::CreateBackBuffer()
{
	HRESULT hr;

	hr = SwapChain->GetBuffer(0, __uuidof(IDXGISurface), (void**)&surface);
	assert(SUCCEEDED(hr));

	D2D1_BITMAP_PROPERTIES1 bp;
	bp.pixelFormat.format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bp.pixelFormat.alphaMode = D2D1_ALPHA_MODE_IGNORE;
	bp.dpiX = 96;
	bp.dpiY = 96;
	bp.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
	bp.colorContext = NULL;
	
	hr = deviceContext->CreateBitmapFromDxgiSurface(surface, &bp, &bitmap);
	assert(SUCCEEDED(hr));

	deviceContext->SetTarget(bitmap);

	hr = deviceContext->CreateSolidColorBrush(D2D1::ColorF(1, 1, 1), &brush);
	assert(SUCCEEDED(hr));

	writeFactory->CreateTextFormat
	(
		L"����ü",
		NULL,
		DWRITE_FONT_WEIGHT_SEMI_BOLD,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		15,
		L"ko",
		&format
	);


}

void DirectWrite::DeleteBackBuffer()
{
	deviceContext->SetTarget(NULL);

	SAFE_RELEASE(brush);
	SAFE_RELEASE(format);

	SAFE_RELEASE(bitmap);
	SAFE_RELEASE(surface);
}
void DirectWrite::RenderText(wstring text, Vector2 pos, int R, int G, int B, float size)
{
	ID2D1SolidColorBrush* brush;
	deviceContext->CreateSolidColorBrush(D2D1::ColorF((float)(R/255), (float)(G/255), (float)(B/255)), &brush);

	IDWriteTextLayout* layout = nullptr;
	HRESULT hr = writeFactory->CreateTextLayout
	(
		text.c_str(),
		text.length(),
		format,
		(float)text.length() * size,
		(float)size,
		&layout
	);
	assert(SUCCEEDED(hr));

	DWRITE_TEXT_RANGE range;
	range.startPosition = 0;
	range.length = text.length();

	layout->SetFontSize((float)size, range);
	//	layout->SetTextAlignment(align);

	deviceContext->SetTransform(D2D1::Matrix3x2F::Identity());

	deviceContext->DrawTextLayout(D2D1::Point2F(pos.x, pos.y), layout,brush);

	SAFE_RELEASE(layout);


}
void DirectWrite::RenderText(wstring & text, RECT & rect)
{
	
	D2D1_RECT_F rt;
	rt.left = (float)rect.left;
	rt.top = (float)rect.top;
	rt.right = (float)rect.right;
	rt.bottom = (float)rect.bottom;

	deviceContext->DrawTextW(text.c_str(), text.length(), format, &rt, brush);

}
void DirectWrite::DrawLine(Vector2 start, Vector2 end, float strokeWidth)
{
	ID2D1SolidColorBrush* brush;
	deviceContext->CreateSolidColorBrush(D2D1::ColorF(0, 1, 0), &brush);

	deviceContext->SetTransform(D2D1::Matrix3x2F::Identity());

	Vector2 startPos = start;
	Vector2 endPos = end;


	deviceContext->DrawLine(D2D1::Point2F(startPos.x, startPos.y), D2D1::Point2F(endPos.x, endPos.y), brush, strokeWidth);
	SAFE_RELEASE(brush);
}

void DirectWrite::SkewRectangle(RECT rect, D2D1::ColorF color, float angle, float angle2)
{
	D2D1_RECT_F rt;
	rt.left = (float)rect.left;
	rt.top = (float)rect.top;
	rt.right = (float)rect.right;
	rt.bottom = (float)rect.bottom;

	D2D1_POINT_2F offset;
	offset.x = rect.left  + (rect.right  - rect.left) *0.5f;
	offset.y = rect.top   + (rect.bottom - rect.top)  *0.5f;

	ID2D1SolidColorBrush* brush;
	deviceContext->CreateSolidColorBrush(color, &brush);
	deviceContext->SetTransform(D2D1::Matrix3x2F::Rotation(angle, offset));
	deviceContext->SetTransform(D2D1::Matrix3x2F::Skew(angle2, 0, offset));
	deviceContext->FillRectangle(D2D1::RectF((float)rect.left, (float)rect.top, (float)rect.right, (float)rect.bottom), brush);

	SAFE_RELEASE(brush);


}
