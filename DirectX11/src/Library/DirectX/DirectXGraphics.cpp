#include "DirectXGraphics.h"
#include "../WindowsAPI/WindowsAPI.h"

#include "../Shader/ShaderManager.h"

#pragma comment(lib,"d3d11.lib")

//D3D11_BUFFER_DESC
//https://docs.microsoft.com/ja-jp/windows/win32/api/d3d11/ns-d3d11-d3d11_buffer_desc
//D3D11_SUBRESOURCE_DATA
//https://docs.microsoft.com/en-us/windows/win32/api/d3d11/ns-d3d11-d3d11_subresource_data

namespace engine
{
	DirectXGraphics::~DirectXGraphics()
	{
		Release();
	}

	bool DirectXGraphics::Init()
	{
		if (!CreateDeviceAndSwapChain()) { return false; }

		if (!CreateRenderTargetView()) { return false; }

		if (!CreateDepthAndStencilView()) { return false; }

		if (!CreateBlendState()) { return false; }

		SetUpViewPort();

		return true;
	}

	void DirectXGraphics::StartRendering()
	{
		//RenderTargetViewの初期化
		m_context->ClearRenderTargetView(
			m_renderTargetView,
			m_clearColor);

		//DepthStenciの初期化
		m_context->ClearDepthStencilView(
			m_depthStencilView,
			D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
			1.0f,
			0);

	}

	void DirectXGraphics::FinishRendering()
	{
		//バッファの切り替え
		m_swapChain->Present(0, 0);
	}

	void DirectXGraphics::SetUpBlendState()
	{
		FLOAT blendFactor[4] = { D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO };
		m_context->OMSetBlendState(m_blendState, blendFactor, 0xffffffff);
	}

	void DirectXGraphics::SetUpContext(const std::string& v_shader_name_, const std::string& p_shader_name_)
	{
		m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		m_context->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

		m_context->VSSetShader(ShaderManager::GetInstance()->GetVertexInterface(v_shader_name_), NULL, 0);
		m_context->PSSetShader(ShaderManager::GetInstance()->GetPixelInterface(p_shader_name_), NULL, 0);
	}

	bool DirectXGraphics::CreateDeviceAndSwapChain()
	{
		HWND windowHandle = FindWindow(WindowsAPI::m_windowClassName.c_str(), nullptr);
		RECT rect;
		GetClientRect(windowHandle, &rect);

		//SwapChainの設定
		DXGI_SWAP_CHAIN_DESC dxgi;
		ZeroMemory(&dxgi, sizeof(DXGI_SWAP_CHAIN_DESC));
		dxgi.BufferCount = 1;
		dxgi.BufferDesc.Width = rect.right - rect.left;
		dxgi.BufferDesc.Height = rect.bottom - rect.top;
		dxgi.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		dxgi.BufferDesc.RefreshRate.Numerator = 60;
		dxgi.BufferDesc.RefreshRate.Denominator = 1;
		dxgi.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		dxgi.OutputWindow = windowHandle;
		dxgi.SampleDesc.Count = 1;
		dxgi.SampleDesc.Quality = 0;
		dxgi.Windowed = true;

		//DeviceとSwapChainの生成
		if (FAILED(D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			0,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&dxgi,
			&m_swapChain,
			&m_device,
			&m_fetureLevel,
			&m_context)))
		{
			return false;
		}

		return true;
	}

	bool DirectXGraphics::CreateRenderTargetView()
	{
		//描画対象のバッファをSwapChain取得
		ID3D11Texture2D* backBuffer;
		if (FAILED(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer)))
		{
			return false;
		}

		//RenderTargetViewの作成
		if (FAILED(m_device->CreateRenderTargetView(backBuffer, NULL, &m_renderTargetView)))
		{
			return false;
		}

		//参照カウンター減少用
		backBuffer->Release();

		return true;
	}

	bool DirectXGraphics::CreateDepthAndStencilView()
	{
		HWND windowHandle = FindWindow(WindowsAPI::m_windowClassName.c_str(), nullptr);
		RECT rect;
		GetClientRect(windowHandle, &rect);

		//DepthStencil用のテクスチャ情報の登録
		D3D11_TEXTURE2D_DESC textureDesc;
		ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
		textureDesc.Width = rect.right - rect.left;
		textureDesc.Height = rect.bottom - rect.top;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		//DepthStencil用のテクスチャの作成
		ID3D11Texture2D* depthStencilTexture;
		if (FAILED(m_device->CreateTexture2D(&textureDesc, NULL, &depthStencilTexture)))
		{
			return false;
		}

		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilDesc;
		ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
		depthStencilDesc.Format = textureDesc.Format;
		depthStencilDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilDesc.Texture2D.MipSlice = 0;

		if (FAILED(m_device->CreateDepthStencilView(
			depthStencilTexture,
			&depthStencilDesc,
			&m_depthStencilView)))
		{
			return false;
		}

		return true;
	}

	bool DirectXGraphics::CreateBlendState()
	{
		D3D11_BLEND_DESC blendDesc;
		ZeroMemory(&blendDesc, sizeof(blendDesc));
		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;
		blendDesc.RenderTarget[0].BlendEnable = TRUE;

		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		if (FAILED(DirectXGraphics::GetInstance()->GetDevice()->CreateBlendState(&blendDesc, &m_blendState)))
		{
			return false;
		}

		return true;
	}

	void DirectXGraphics::SetUpViewPort()
	{
		HWND windowHandle = FindWindow(WindowsAPI::m_windowClassName.c_str(), nullptr);
		RECT rect;
		GetClientRect(windowHandle, &rect);

		D3D11_VIEWPORT viewPort;
		viewPort.TopLeftX = 0;
		viewPort.TopLeftY = 0;
		viewPort.Width = rect.right - rect.left;
		viewPort.Height = rect.bottom - rect.top;
		viewPort.MinDepth = 0.0f;
		viewPort.MaxDepth = 1.0f;

		m_context->RSSetViewports(
			1,
			&viewPort);
	}

	void DirectXGraphics::Release()
	{
		if (m_device != nullptr) { m_device->Release(); }
		if (m_swapChain != nullptr) { m_swapChain->Release(); }
		if (m_context != nullptr) { m_context->Release(); }
		if (m_renderTargetView != nullptr) { m_renderTargetView->Release(); }
		if (m_depthStencilView != nullptr) { m_depthStencilView->Release(); }
	}

}