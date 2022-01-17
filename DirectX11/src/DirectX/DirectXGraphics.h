﻿#ifndef DIRECTX_GRAPHICS_H
#define DIRECTX_GRAPHICS_H

#include <d3d11.h>
#include <DirectXMath.h>

class DirectXGraphics
{
public:
	/**
	* @brief DirectGraphicsの初期化
	* @return 成功したらtrue
	* @note デバイス等の生成
	*/
	bool Init();

	/**
	* @brief DirectGraphicsの解放
	*/
	void Releace();

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


	//アクセサ

	ID3D11Device* GetDevice() const { return m_device; }

	ID3D11Buffer* GetConstantBuffer() const { return m_constantBuffer; }

	ID3D11DeviceContext* GetContext() const { return m_context; }

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
	* @brief ViewPortの設定
	*/
	void SetUpViewPort();

	bool CreateConstatBuffer();

	bool CreateSheder();

private:
	ID3D11Device* m_device;
	IDXGISwapChain* m_swapChain;
	D3D_FEATURE_LEVEL m_fetureLevel;
	ID3D11Buffer* m_constantBuffer;
	ID3D11DeviceContext* m_context;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11DepthStencilView* m_depthStencilView;


	//バッファ初期化色
	float m_clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
};

#endif // !DIRECTX_GRAPHICS_H
