#include "Graphics2D.h"

#pragma comment(lib,"d3d11.lib")

namespace engine
{
	Graphics2D::~Graphics2D()
	{
		Relese();
	}

	void engine::Graphics2D::DrawPorigon(const Vec2f& pos_, const float& width_, const float& height_, const Vec3f& color_, const float& alpha_, const float& degree_)
	{
		ID3D11DeviceContext* context = DirectXGraphics::GetInstance()->GetContext();

		// 各bufferの更新
		SetUpBuffers(context, pos_, width_, height_, color_, alpha_, degree_);

		// アルファ値情報の設定
		DirectXGraphics::GetInstance()->SetUpBlendState();

		// GPUへ送るデータの設定
		DirectXGraphics::GetInstance()->SetUpContext(m_vShaderName, m_pShaderName);
		SetUpContext(context);

		context->DrawIndexed(3, 0, 0);
	}

	void Graphics2D::DrawRect(const Vec2f& pos_, const float& width_, const float& height_, const Vec3f& color_, const float& alpha_, const float& degree_)
	{
		ID3D11DeviceContext* context = DirectXGraphics::GetInstance()->GetContext();

		// 各bufferの更新
		SetUpBuffers(context, pos_, width_, height_, color_, alpha_, degree_);

		// アルファ値情報の設定
		DirectXGraphics::GetInstance()->SetUpBlendState();

		// GPUへ送るデータの設定
		DirectXGraphics::GetInstance()->SetUpContext(m_vShaderName, m_pShaderName);
		SetUpContext(context);

		context->DrawIndexed(4, 0, 0);
	}

	void Graphics2D::Init()
	{
		CreateRect();
	}

	void Graphics2D::CreateRect()
	{
		// 頂点データ作成
		CustomVertex vertexList[]
		{
			{ { -0.5f,  0.5f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
			{ {  0.5f,  0.5f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
			{ { -0.5f, -0.5f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
			{ {  0.5f, -0.5f, 0.0f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f } }
		};

		// 頂点番号
		UWORD index[]
		{
			0, 1, 2, 3,
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
			return;
		}

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
			return;
		}

		D3D11_INPUT_ELEMENT_DESC vertexDesc[]{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		if (FAILED(DirectXGraphics::GetInstance()->GetDevice()->CreateInputLayout(
			vertexDesc,
			ARRAYSIZE(vertexDesc),
			ShaderManager::GetInstance()->GetShaderData(m_vShaderName),
			ShaderManager::GetInstance()->GetShaderSize(m_vShaderName),
			&m_inputLayout)))
		{
			return;
		}

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
			return;
		}
	}

	void Graphics2D::Relese()
	{
		if (m_vertexBuffer != nullptr) { m_vertexBuffer->Release(); }
		if (m_indexBuffer != nullptr) { m_indexBuffer->Release(); }
		if (m_inputLayout != nullptr) { m_inputLayout->Release(); }
		if (m_constantBuffer != nullptr) { m_constantBuffer->Release(); }
	}
	void Graphics2D::SetUpBuffers(ID3D11DeviceContext* context_, const Vec2f& pos_, const float& width_, const float& height_, const Vec3f& color_, const float& alpha_, const float& degree_)
	{
		// constantBufferの更新
		DirectX::XMMATRIX world = Matrix::GetInstance()->CreateWorldMatrix2D(pos_, width_, height_, degree_);

		ConstantBuffer constantBuffer;
		DirectX::XMStoreFloat4x4(&constantBuffer.world, DirectX::XMMatrixTranspose(world));
		constantBuffer.viewport = Matrix::GetInstance()->GetViewportMatrix();

		context_->UpdateSubresource(m_constantBuffer, 0, NULL, &constantBuffer, 0, 0);

		// VertexBufferの更新
		D3D11_MAPPED_SUBRESOURCE mappedRes{};
		CustomVertex vertexList[]
		{
			{ { -0.5f,  0.5f, 0.0f, 1.0f }, { color_.x, color_.y, color_.z, alpha_ } },
			{ {  0.5f,  0.5f, 0.0f, 1.0f }, { color_.x, color_.y, color_.z, alpha_ } },
			{ { -0.5f, -0.5f, 0.0f, 1.0f }, { color_.x, color_.y, color_.z, alpha_ } },
			{ {  0.5f, -0.5f, 0.0f, 1.0f }, { color_.x, color_.y, color_.z, alpha_ } }
		};

		context_->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedRes);
		memcpy(mappedRes.pData, vertexList, sizeof(vertexList));
		context_->Unmap(m_vertexBuffer, 0);
	}
	void Graphics2D::SetUpContext(ID3D11DeviceContext* context_)
	{
		DirectXGraphics::GetInstance()->SetUpContext(m_vShaderName, m_pShaderName);

		UINT strides = sizeof(CustomVertex);
		UINT offsets = 0;
		context_->IASetInputLayout(m_inputLayout);
		context_->IASetVertexBuffers(0, 1, &m_vertexBuffer, &strides, &offsets);
		context_->VSSetConstantBuffers(0, 1, &m_constantBuffer);
		context_->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R16_UINT, 0);
		context_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	}
}
