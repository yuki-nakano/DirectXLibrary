#include "Direct2D.h"

#include "DirectXGraphics.h"
#include "../WindowsAPI/WindowsAPI.h"

#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"dwrite.lib")

namespace engine
{
	Direct2D::~Direct2D()
	{
		m_deviceDxgi->Release();
		m_factory->Release();
		m_writeFactory->Release();
		m_textFormat->Release();
		m_renderTarget2D->Release();
		m_solidBrush->Release();
		m_backBuffer->Release();

		m_deviceDxgi = nullptr;
		m_factory = nullptr;
		m_writeFactory = nullptr;
		m_textFormat = nullptr;
		m_renderTarget2D = nullptr;
		m_solidBrush = nullptr;
		m_backBuffer = nullptr;
	}

	bool Direct2D::Init()
	{
		D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_factory);

		if (FAILED(DirectXGraphics::GetInstance()->GetDevice()->QueryInterface(IID_PPV_ARGS(&m_deviceDxgi)))) { return false; }

		if (FAILED(DirectXGraphics::GetInstance()->GetSwapChain()->GetBuffer(0, IID_PPV_ARGS(&m_backBuffer)))) { return false; }

		HWND windowHandle = FindWindow(WindowsAPI::m_windowClassName.c_str(), nullptr);
		UINT dpi = GetDpiForWindow(windowHandle);

		if (!GetClientRect(windowHandle, &m_windowRect)) { return false; }

		D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
			dpi, dpi);

		if (FAILED(m_factory->CreateDxgiSurfaceRenderTarget(m_backBuffer, &props, &m_renderTarget2D))) { return false; }

		DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&m_writeFactory));

		// フォント設定
		if (FAILED(m_writeFactory->CreateTextFormat(L"メイリオ", nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 20, L"", &m_textFormat))) { return false; }

		// テキスト配置
		if (FAILED(m_textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING))) { return false; }

		// カラー設定
		if (FAILED(m_renderTarget2D->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black, 1.0f), &m_solidBrush))) { return false; }

		return false;
	}

	void Direct2D::DrawTextData(const std::wstring& text_, Vec2f pos_)
	{
		m_renderTarget2D->DrawText(text_.c_str(), text_.size(), m_textFormat, D2D1::RectF(pos_.x, pos_.y, m_windowRect.right, m_windowRect.bottom), m_solidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
	}
}