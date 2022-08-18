#include "DirectXTexture.h"

#include "../DirectX/DirectXGraphics.h"
#include "../Shader/ShaderManager.h"
#include "../DirectX/Matrix.h"
#include "../Lib/WICTextureLoader11.h"

namespace engine
{
	DirectXTexture::~DirectXTexture()
	{
		Relese();
	}

	bool DirectXTexture::Init()
	{
		if (!CreateVertexBuffer()) { return false; }

		if (!CreateIndexBuffer()) { return false; }

		if (!CreateInputLayout()) { return false; }

		if (!CreateConstantBuffer()) { return false; }

		if (!CreateSampler()) { return false; }

		return true;
	}

	bool DirectXTexture::LoadTexture(const std::wstring& file_name_, const std::string& name_)
	{
		ID3D11ShaderResourceView* textureView;
		if (FAILED(DirectX::CreateWICTextureFromFile(DirectXGraphics::GetInstance()->GetDevice(), file_name_.c_str(), nullptr, &textureView)))
		{
			return false;
		}

		texList.emplace(name_, textureView);

		return true;
	}

	void DirectXTexture::DrawTexture(const std::string& name_, const Vec2f& pos_, const float& width_, const float& height_, const float& degree_, const float& alpha_)
	{
		ID3D11DeviceContext* context = DirectXGraphics::GetInstance()->GetContext();

		// bufferの更新
		DirectX::XMMATRIX world = Matrix::GetInstance()->CreateWorldMatrix2D(pos_, width_, height_, degree_);

		ConstantBuffer constantBuffer;
		DirectX::XMStoreFloat4x4(&constantBuffer.world, DirectX::XMMatrixTranspose(world));
		constantBuffer.viewport = Matrix::GetInstance()->GetViewportMatrix();

		context->UpdateSubresource(m_constantBuffer, 0, NULL, &constantBuffer, 0, 0);

		D3D11_MAPPED_SUBRESOURCE mappedRes{};
		CustomVertex vertexList[]
		{
			{ { -0.5f,  0.5f, 0.0f },{ 0.0f, 0.0f, 0.0f, alpha_ }, { 0.0f, 0.0f } },
			{ {  0.5f,  0.5f, 0.0f },{ 0.0f, 0.0f, 0.0f, alpha_ }, { 1.0f, 0.0f } },
			{ { -0.5f, -0.5f, 0.0f },{ 0.0f, 0.0f, 0.0f, alpha_ }, { 0.0f, 1.0f } },
			{ {  0.5f, -0.5f, 0.0f },{ 0.0f, 0.0f, 0.0f, alpha_ }, { 1.0f, 1.0f } }
		};

		context->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedRes);
		memcpy(mappedRes.pData, vertexList, sizeof(vertexList));
		context->Unmap(m_vertexBuffer, 0);

		// 透過の設定
		DirectXGraphics::GetInstance()->SetUpBlendState();

		// GPUへ送るデータの設定
		DirectXGraphics::GetInstance()->SetUpContext(m_vShaderName, m_pShaderName);

		UINT strides = sizeof(CustomVertex);
		UINT offsets = 0;
		context->IASetInputLayout(m_inputLayout);
		context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &strides, &offsets);
		context->VSSetConstantBuffers(0, 1, &m_constantBuffer);
		context->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R16_UINT, 0);
		context->PSSetShaderResources(0, 1, &texList.at(name_));
		context->PSSetSamplers(0, 1, &m_sampler);

		context->DrawIndexed(4, 0, 0);
	}

	void DirectXTexture::ReleseTexture(const std::string& name_)
	{
		texList.at(name_)->Release();
	}

	bool DirectXTexture::CreateVertexBuffer()
	{
		// 頂点データ作成
		CustomVertex vertexList[]
		{
			{ { -0.5f,  0.5f, 0.0f },{ 0.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
			{ {  0.5f,  0.5f, 0.0f },{ 0.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } },
			{ { -0.5f, -0.5f, 0.0f },{ 0.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } },
			{ {  0.5f, -0.5f, 0.0f },{ 0.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } }

		};

		D3D11_BUFFER_DESC vertexBufferDesc = {
			sizeof(CustomVertex) * 4,
			D3D11_USAGE_DYNAMIC,
			D3D11_BIND_VERTEX_BUFFER,
			D3D11_CPU_ACCESS_WRITE,
			0,
			sizeof(CustomVertex) };

		D3D11_SUBRESOURCE_DATA vertexSubresData = {
			vertexList,
			0,
			0 };

		if (FAILED(DirectXGraphics::GetInstance()->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexSubresData, &m_vertexBuffer)))
		{
			return false;
		}

		return true;
	}

	bool DirectXTexture::CreateIndexBuffer()
	{
		UWORD index[]
		{
			0, 1, 2, 3,
		};

		D3D11_BUFFER_DESC indexBufferDesc = {
			sizeof(UWORD) * 4,
			D3D11_USAGE_DEFAULT,
			D3D11_BIND_INDEX_BUFFER,
			0,
			0,
			0 };

		D3D11_SUBRESOURCE_DATA indexSubresData = {
			index,
			0,
			0 };

		if (FAILED(DirectXGraphics::GetInstance()->GetDevice()->CreateBuffer(&indexBufferDesc, &indexSubresData, &m_indexBuffer)))
		{
			return false;
		}

		return true;
	}

	bool DirectXTexture::CreateInputLayout()
	{
		D3D11_INPUT_ELEMENT_DESC vertexDesc[]{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		if (FAILED(DirectXGraphics::GetInstance()->GetDevice()->CreateInputLayout(
			vertexDesc,
			ARRAYSIZE(vertexDesc),
			ShaderManager::GetInstance()->GetShaderData(m_vShaderName),
			ShaderManager::GetInstance()->GetShaderSize(m_vShaderName),
			&m_inputLayout)))
		{
			return false;
		}

		return true;
	}

	bool DirectXTexture::CreateConstantBuffer()
	{
		D3D11_BUFFER_DESC constantBufferDesc = {
			sizeof(ConstantBuffer),
			D3D11_USAGE_DEFAULT,
			D3D11_BIND_CONSTANT_BUFFER,
			0,
			0,
			0
		};

		if (FAILED(DirectXGraphics::GetInstance()->GetDevice()->CreateBuffer(&constantBufferDesc, NULL, &m_constantBuffer)))
		{
			return false;
		}

		return true;
	}

	bool DirectXTexture::CreateSampler()
	{
		//https://docs.microsoft.com/en-us/windows/win32/api/d3d11/ns-d3d11-d3d11_sampler_desc
		D3D11_SAMPLER_DESC smpDesc;
		ZeroMemory(&smpDesc, sizeof(D3D11_SAMPLER_DESC));
		smpDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		smpDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		smpDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		smpDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

		if (FAILED(DirectXGraphics::GetInstance()->GetDevice()->CreateSamplerState(&smpDesc, &m_sampler)))
		{
			return false;
		}

		return true;
	}

	void DirectXTexture::Relese()
	{
		for (auto tex : texList)
		{
			tex.second->Release();
		}

		if (m_vertexBuffer != nullptr) { m_vertexBuffer->Release(); }
		if (m_indexBuffer != nullptr) { m_indexBuffer->Release(); }
		if (m_inputLayout != nullptr) { m_inputLayout->Release(); }
		if (m_constantBuffer != nullptr) { m_constantBuffer->Release(); }
		if (m_sampler != nullptr) { m_sampler->Release(); }
	}
}