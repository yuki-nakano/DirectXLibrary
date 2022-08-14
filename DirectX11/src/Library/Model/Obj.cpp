#include "Obj.h"

#include <iostream>
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

		if (!AnalyzeObjData(file_name_, customVertex)) { return false; }

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
			sizeof(UWORD) * (UINT)m_indexList.size(),
			D3D11_USAGE_DEFAULT,
			D3D11_BIND_INDEX_BUFFER,
			0,
			0,
			0 };

		D3D11_SUBRESOURCE_DATA indexSubresData = {
			&m_indexList[0],
			0,
			0 };

		if (FAILED(DirectXGraphics::GetInstance()->GetDevice()->CreateBuffer(&indexBufferDesc, &indexSubresData, &m_indexBuffer)))
		{
			return false;
		}

		D3D11_INPUT_ELEMENT_DESC vertexDesc[]{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXTURE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
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

	bool Obj::AnalyzeObjData(const std::string& file_name_, std::vector<CustomVertex>& custom_vertex_)
	{
		std::ifstream file(file_name_);
		if (!file) { return false; }

		ObjData objData{};
		std::string currentMtlName{};

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
			else if (key.compare("vt") == 0)
			{
				TextureUV uv{};
				std::string num{};

				std::getline(ss, num, ' ');
				uv[0] = std::stof(num);

				std::getline(ss, num, ' ');
				uv[1] = std::stof(num) * -1;

				objData.texDataList.push_back(uv);
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
						if (num == "")
						{
							m_haveTexture = false;
							count++;
							continue;
						}

						element[count] = std::stoi(num) - 1;
						count++;
					}

					CustomVertex customVertex{};

					int vertexNum = element.at(0);
					customVertex.vecesData = objData.vecesDataList.at(vertexNum);

					if (m_haveTexture)
					{
						int texNum = element.at(1);
						customVertex.texData = objData.texDataList.at(texNum);
					}

					int normalNum = element.at(2);
					customVertex.nomalData = objData.nomalDataList.at(normalNum);

					custom_vertex_.push_back(customVertex);

					m_indexList.push_back((UWORD)custom_vertex_.size() - 1);

					m_mtlIndex.at(currentMtlName).push_back((UWORD)custom_vertex_.size() - 1);
				}
			}
			else if (key.compare("mtllib") == 0)
			{
				std::string name{};
				std::getline(ss, name, ' ');

				m_mtlFileName = name;
			}
			else if (key.compare("usemtl") == 0)
			{
				std::string name{};
				std::getline(ss, name, ' ');

				currentMtlName = name;

				std::vector<UWORD> index{};
				m_mtlIndex.emplace(name, index);
			}
		}

		m_vertexNum = m_indexList.size();

		return true;
	}

	bool Obj::AnalyzeMtlData()
	{
		if (m_mtlFileName == "none") { return true; }

		std::ifstream file(m_mtlFileName);
		if (!file) { return false; }

		std::string currentMtlName{};

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
			else if (key.compare("Kd") == 0)
			{
				std::array<float, 3 > param{};
				std::string num{};
				int count{ 0 };
				while (std::getline(ss, num, ' '))
				{
					param[count] = std::stof(num);
					count++;
				}
			}
			else if (key.compare("Ks") == 0)
			{

			}
			else if (key.compare("Ka") == 0)
			{

			}
			else if (key.compare("d") == 0)
			{

			}
			else if (key.compare("Tr") == 0)
			{

			}
			else if (key.compare("Ni") == 0)
			{

			}
			else if (key.compare("illum") == 0)
			{

			}
			else if (key.compare("tf") == 0)
			{

			}
			else if (key.compare("Ns") == 0)
			{

			}
			else if (key.compare("sharpness") == 0)
			{

			}

			else if (key.compare("newmtl"))
			{
				std::string name{};
				std::getline(ss, name, ' ');

				currentMtlName = name;
			}
		}

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