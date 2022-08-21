#ifndef DIRECT2D_H

#include <d3d11.h>
#include <d2d1.h>
#include <dwrite.h>
#include <string>

#include "../Utility/Singleton.h" 
#include "DirectXUtility.h"

namespace engine
{
	class Direct2D : public Singleton<Direct2D>
	{
	private:
		friend class Singleton<Direct2D>;

		Direct2D() = default;
		~Direct2D() {};

	public:
		/**
		* @brief テキストデータの初期化
		*/
		bool Init();

		/**
		* @brief 文字列の描画
		* @param text_ 描画する文字列
		* @param pos_ 描画する座標
		*/
		void DrawTextData(const std::wstring& text_, Vec2f pos_);

		// アクセサ

		ID2D1RenderTarget* GetRenderTarget2D() { return m_renderTarget2D; }

	private:
		IDXGIDevice* m_deviceDxgi{};
		ID2D1Factory* m_factory{};
		IDWriteFactory* m_writeFactory{};
		IDWriteTextFormat* m_textFormat{};
		ID2D1RenderTarget* m_renderTarget2D{};
		ID2D1SolidColorBrush* m_solidBrush{};
		IDXGISurface* m_backBuffer{};

		RECT m_windowRect{};
	};
}

#endif // !DIRECT2D_H
