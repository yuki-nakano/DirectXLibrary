﻿#include "DirectXTexture.h"

#include "../DirectX/DirectXGraphics.h"
#include "../Shader/ShaderManager.h"
#include "../DirectX/Matrix.h"
#include "../Lib/WICTextureLoader11.h"

namespace engine
{
	DirectXTexture::DirectXTexture()
	{
	}

	DirectXTexture::~DirectXTexture()
	{
	}

	bool DirectXTexture::Init()
	{
		if (CreateVertexBuffer()) { return false; }

		if (CreateIndexBuffer()) { return false; }

		if (CreateInputLayout()) { return false; }

		if (CreateConstantBuffer()) { return false; }

		if (CreateSampler()) { return false; }

		return true;
	}

	void DirectXTexture::Relese()
	{
		for (auto tex : texList)
		{
			tex.second->Release();
		}

		m_vertexBuffer->Release();
		m_indexBuffer->Release();
		m_inputLayout->Release();
		m_constantBuffer->Release();
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

	void DirectXTexture::DrawTexture(const std::string& name_, float pos_x_, float pos_y_, float width_, float height_, float angle_)
	{
		ID3D11DeviceContext* context = DirectXGraphics::GetInstance()->GetContext();

		DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
		DirectX::XMMATRIX rotateZ = DirectX::XMMatrixRotationZ(angle_ / (180.0f * 3.14f));
		DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(width_, height_, 1.0f);
		DirectX::XMMATRIX translate = DirectX::XMMatrixTranslation(pos_x_, pos_y_, 0);

		world = scale * rotateZ * translate;

		ConstantBuffer constantBuffer;
		DirectX::XMStoreFloat4x4(&constantBuffer.world, DirectX::XMMatrixTranspose(world));
		constantBuffer.viewport = Matrix::GetInstance()->GetViewportMatrix();

		context->UpdateSubresource(m_constantBuffer, 0, NULL, &constantBuffer, 0, 0);

		UINT strides = sizeof(CustomVertex);
		UINT offsets = 0;
		context->IASetInputLayout(m_inputLayout);
		context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &strides, &offsets);
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		context->VSSetConstantBuffers(0, 1, &m_constantBuffer);
		context->VSSetShader(ShaderManager::GetInstance()->GetVertexInterface(m_vShaderName), NULL, 0);
		context->PSSetShader(ShaderManager::GetInstance()->GetPixelInterface(m_pShaderName), NULL, 0);
		context->OMSetRenderTargets(1, DirectXGraphics::GetInstance()->GetRenderTargetView(), DirectXGraphics::GetInstance()->GetDepthStencilView());
		context->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R16_UINT, 0);
		context->PSSetShaderResources(0, 1, &texList.at(name_));
		context->PSSetSamplers(0, 1, &m_sampler);

		context->DrawIndexed(6, 0, 0);
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
			{ { -0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f } },
			{ {  0.5f,  0.5f, 0.0f }, { 1.0f, 0.0f } },
			{ { -0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f } },
			{ {  0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f } }

		};

		D3D11_BUFFER_DESC vertexBufferDesc = {
			sizeof(CustomVertex) * 4,
			D3D11_USAGE_DEFAULT,
			D3D11_BIND_VERTEX_BUFFER,
			0,
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

		return false;
	}

	bool DirectXTexture::CreateIndexBuffer()
	{
		UWORD index[]
		{
			0, 1, 2, 3,
		};

		D3D11_BUFFER_DESC indexBufferDesc = {
			sizeof(UWORD) * 6,
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
}