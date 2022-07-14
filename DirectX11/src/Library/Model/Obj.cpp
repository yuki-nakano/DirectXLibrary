#include "Obj.h"

#include <fstream>
#include <sstream>
#include <DirectXMath.h>

#include "../Shader/ShaderManager.h"
#include "../DirectX/Matrix.h"
#include "../DirectX/DirectXGraphics.h"


namespace engine
{
	Obj::~Obj()
	{
		Relese();
	}

	bool Obj::Load(const std::string& file_name_)
	{
		std::vector<CustomVertex> customVertex;

		if (!AnalyzeData(file_name_, customVertex)) { return false; }

		D3D11_BUFFER_DESC vertexBufferDesc = {
			sizeof(CustomVertex) * (UINT)m_vertexNum,
			D3D11_USAGE_DEFAULT,
			D3D11_BIND_VERTEX_BUFFER,
			0,
			0,
			0 };

		D3D11_SUBRESOURCE_DATA vertexSubresData = {
			&customVertex[0],
			0,
			0 };

		if (FAILED(DirectXGraphics::GetInstance()->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexSubresData, &m_vertexBuffer)))
		{
			return false;
		}

		D3D11_BUFFER_DESC indexBufferDesc = {
			sizeof(UWORD) * (UINT)m_index.size(),
			D3D11_USAGE_DEFAULT,
			D3D11_BIND_INDEX_BUFFER,
			0,
			0,
			0 };

		D3D11_SUBRESOURCE_DATA indexSubresData = {
			&m_index[0],
			0,
			0 };

		if (FAILED(DirectXGraphics::GetInstance()->GetDevice()->CreateBuffer(&indexBufferDesc, &indexSubresData, &m_indexBuffer)))
		{
			return false;
		}

		D3D11_INPUT_ELEMENT_DESC vertexDesc[]{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
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

		D3D11_BUFFER_DESC constantBufferDesc = {
			sizeof(ConstantBuffer),
			D3D11_USAGE_DEFAULT,
			D3D11_BIND_CONSTANT_BUFFER,
			0,
			0,
			sizeof(ConstantBuffer)
		};

		if (FAILED(DirectXGraphics::GetInstance()->GetDevice()->CreateBuffer(&constantBufferDesc, NULL, &m_constantBuffer)))
		{
			return false;
		}

		return true;
	}

	void Obj::Render(Vec3f pos_, Vec3f rote_, Vec3f scale_)
	{
		ID3D11DeviceContext* context = DirectXGraphics::GetInstance()->GetContext();

		DirectX::XMMATRIX world = Matrix::GetInstance()->CreateWorldMatrix(pos_, rote_, scale_);
		DirectX::XMMATRIX view = Matrix::GetInstance()->CreateViewMatrix();
		DirectX::XMMATRIX proj = Matrix::GetInstance()->CreateProjMatrix();

		ConstantBuffer constantBuffer;
		DirectX::XMStoreFloat4x4(&constantBuffer.world, DirectX::XMMatrixTranspose(world));
		DirectX::XMStoreFloat4x4(&constantBuffer.view, DirectX::XMMatrixTranspose(view));
		DirectX::XMStoreFloat4x4(&constantBuffer.proj, DirectX::XMMatrixTranspose(proj));

		DirectX::XMVECTOR light = Matrix::GetInstance()->CreateLightPosMatrix();
		DirectX::XMStoreFloat4(&constantBuffer.LightVec, light);
		constantBuffer.LightCol = Matrix::GetInstance()->CreateLightColMatrix();

		context->UpdateSubresource(m_constantBuffer, 0, NULL, &constantBuffer, 0, 0);

		UINT strides = sizeof(CustomVertex);
		UINT offsets = 0;
		context->IASetInputLayout(m_inputLayout);
		context->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R16_UINT, 0);
		context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &strides, &offsets);
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		context->VSSetConstantBuffers(0, 1, &m_constantBuffer);
		context->VSSetShader(ShaderManager::GetInstance()->GetVertexInterface(m_vShaderName), NULL, 0);
		context->PSSetShader(ShaderManager::GetInstance()->GetPixelInterface(m_pShaderName), NULL, 0);
		context->OMSetRenderTargets(1, DirectXGraphics::GetInstance()->GetRenderTargetView(), DirectXGraphics::GetInstance()->GetDepthStencilView());
		context->DrawIndexed(m_vertexNum, 0, 0);
	}

	bool Obj::AnalyzeData(const std::string& file_name_, std::vector<CustomVertex>& custom_vertex_)
	{
		std::ifstream file(file_name_);
		if (!file) { return false; }

		ObjData objData{};

		std::string str{};
		while (std::getline(file, str))
		{
			std::stringstream ss{ str };
			std::string key{};
			std::getline(ss, key, ' ');

			if (key.compare("#") == 0)
			{
				continue;
			}
			else if (key.compare("v") == 0)
			{
				VecesData vertex{};
				std::string num{};
				int count{ 0 };
				while (std::getline(ss, num, ' '))
				{
					vertex[count] = std::stof(num);
					count++;
				}
				objData.vecesDataList.push_back(vertex);
			}
			else if (key.compare("vn") == 0)
			{
				NomalData normal{};
				std::string num{};
				int count{ 0 };
				while (std::getline(ss, num, ' '))
				{
					normal[count] = std::stof(num);
					count++;
				}
				objData.nomalDataList.push_back(normal);
			}
			else if (key.compare("f") == 0)
			{
				std::string m_data{};

				while (std::getline(ss, m_data, ' '))
				{
					std::array<int, 3> element;
					std::stringstream data_ss{ m_data };
					std::string num;
					int count{ 0 };
					while (std::getline(data_ss, num, '/'))
					{
						element[count] = std::stoi(num) - 1;
						count++;
					}

					CustomVertex customVertex{};

					int vertexNum = element.at(0);
					customVertex.vecesData = objData.vecesDataList.at(vertexNum);

					int normalNum = element.at(2);
					customVertex.nomalData = objData.nomalDataList.at(normalNum);

					custom_vertex_.push_back(customVertex);

					m_index.push_back((UWORD)custom_vertex_.size() - 1);
				}
			}
		}

		m_vertexNum = m_index.size();

		return true;
	}

	void Obj::Relese()
	{
		if (m_vertexBuffer != nullptr) { m_vertexBuffer->Release(); }
		if (m_indexBuffer != nullptr) { m_indexBuffer->Release(); }
		if (m_constantBuffer != nullptr) { m_constantBuffer->Release(); }
		if (m_inputLayout != nullptr) { m_inputLayout->Release(); }
	}
}