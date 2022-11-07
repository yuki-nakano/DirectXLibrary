#ifndef DIRECTX_GRAPHICS_H
#define DIRECTX_GRAPHICS_H

#include <d3d11.h>
#include <DirectXMath.h>
#include <string>

#include "../Utility/Singleton.h" 

namespace engine
{
	/**
	* @brief グラフィックスの初期化クラス
	*/
	class DirectXGraphics : public Singleton<DirectXGraphics>
	{
	private:
		friend class Singleton<DirectXGraphics>;

		DirectXGraphics() = default;
		~DirectXGraphics();

	public:
		/**
		* @brief DirectGraphicsの初期化
		* @return 成功したらtrue
		* @note デバイス等の生成
		*/
		bool Init();

		/**
		* @brief レンダリングの開始
		* @note バッファの初期化
		*/
		void StartRendering();

		/**
		* @brief レンダリングの終了
		* @note バッファの切り替え
		*/
		void FinishRendering();

		/**
		* @brief BlendStateの設定
		*/
		void SetUpBlendState();

		/**
		* @brief GUI送信用データの設定
		* @param v_shader_name_ 登録したvertexシェーダーの名前
		* @param p_shader_name_ 登録したpixelシェーダーの名前
		*/
		void SetUpContext(const std::string& v_shader_name_, const std::string& p_shader_name_);

		/**
		* @brief GUI送信用データの設定(2D)
		* @param v_shader_name_ 登録したvertexシェーダーの名前
		* @param p_shader_name_ 登録したpixelシェーダーの名前
		*/
		void SetUpContext2D(const std::string& v_shader_name_, const std::string& p_shader_name_);

		//アクセサ

		ID3D11Device* GetDevice() const { return m_device; }
		ID3D11DeviceContext* GetContext() const { return m_context; }
		ID3D11RenderTargetView** GetRenderTargetView() { return &m_renderTargetView; }
		ID3D11DepthStencilView* GetDepthStencilView()const { return m_depthStencilView; }
		IDXGISwapChain* GetSwapChain() const { return m_swapChain; }

	private:
		/**
		* @brief DeviceとSwapChainの作成
		* @return 成功したらtrue
		*/
		bool CreateDeviceAndSwapChain();

		/**
		* @brief RenderTargetViewの作成
		* @return 成功したらtrue
		*/
		bool CreateRenderTargetView();

		/**
		* @brief DepthバッファとStencilバッファの作成
		* @return 成功したらtrue
		*/
		bool CreateDepthAndStencilView();

		/**
		* @brief 画像透過用BlendStateの作成
		* @return 作成成功時true
		*/
		bool CreateBlendState();

		/**
		* @brief ViewPortの設定
		*/
		void SetUpViewPort();

		/**
		* @brief DirectGraphicsの解放
		*/
		void Release();

	private:
		ID3D11Device* m_device{ nullptr };
		IDXGISwapChain* m_swapChain{ nullptr };
		D3D_FEATURE_LEVEL m_fetureLevel{};
		ID3D11DeviceContext* m_context{ nullptr };
		ID3D11RenderTargetView* m_renderTargetView{ nullptr };
		ID3D11DepthStencilView* m_depthStencilView{ nullptr };
		ID3D11BlendState* m_blendState{ nullptr };

		///バッファ初期化色
		float m_clearColor[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
	};
}
#endif // !DIRECTX_GRAPHICS_H
