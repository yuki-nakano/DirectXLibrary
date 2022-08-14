#include "Graphics2D.h"

#pragma comment(lib,"d3d11.lib")

namespace engine
{
	Graphics2D::~Graphics2D()
	{
		Relese();
	}

	void engine::Graphics2D::DrawPorigon(float pos_x_, float pos_y_, float width_, float height_, float angle_)
	{
		ID3D11DeviceContext* context = DirectXGraphics::GetInstance()->GetContext();

		DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
		DirectX::XMMATRIX rotateZ = DirectX::XMMatrixRotationZ(angle_ * (3.14f / 180.0f));
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
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		context->VSSetConstantBuffers(0, 1, &m_constantBuffer);
		context->VSSetShader(ShaderManager::GetInstance()->GetVertexInterface(m_vShaderName), NULL, 0);
		context->PSSetShader(ShaderManager::GetInstance()->GetPixelInterface(m_pShaderName), NULL, 0);
		context->OMSetRenderTargets(1, DirectXGraphics::GetInstance()->GetRenderTargetView(), DirectXGraphics::GetInstance()->GetDepthStencilView());
		context->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R16_UINT, 0);
		context->DrawIndexed(3, 0, 0);
	}

	void Graphics2D::DrawRect(float pos_x_, float pos_y_, float width_, float height_, float angle_)
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
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
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
}
