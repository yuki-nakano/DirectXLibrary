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

		if (!AnalyzeMtlData(file_name_)) { return false; }

		D3D11_BUFFER_DESC vertexBufferDesc = {
			sizeof(CustomVertex) * (UINT)customVertex.size(),
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

		int count{ 0 };
		for (auto index : m_mtlIndex)
		{
			m_indexBufferList.emplace_back();

			D3D11_BUFFER_DESC indexBufferDesc = {
				sizeof(UINT) * (UINT)index.second.size(),
				D3D11_USAGE_DEFAULT,
				D3D11_BIND_INDEX_BUFFER,
				0,
				0,
				0 };

			D3D11_SUBRESOURCE_DATA indexSubresData = {
				&index.second[0],
				0,
				0 };

			if (FAILED(DirectXGraphics::GetInstance()->GetDevice()->CreateBuffer(&indexBufferDesc, &indexSubresData, &m_indexBufferList[count])))
			{
				return false;
			}

			count++;
		}

		D3D11_INPUT_ELEMENT_DESC vertexDesc[]{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXTURE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
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
			0
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

		int count{ 0 };
		for (auto index : m_mtlIndex)
		{
			if (m_isAttachedMtl)
			{
				auto mtlData = m_mtlDataList.at(index.first);
				constantBuffer.mtlAmbient = DirectX::XMFLOAT4(mtlData.Ka.at(0), mtlData.Ka.at(1), mtlData.Ka.at(2), 1.0f);
				constantBuffer.mtlDiffuse = DirectX::XMFLOAT4(mtlData.Kd.at(0), mtlData.Kd.at(1), mtlData.Kd.at(2), 1.0f);
			}
			else
			{
				constantBuffer.mtlAmbient = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
				constantBuffer.mtlDiffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			}

			context->UpdateSubresource(m_constantBuffer, 0, NULL, &constantBuffer, 0, 0);

			// 透過の設定
			DirectXGraphics::GetInstance()->SetUpBlendState();

			// GPUへ送るデータの設定
			DirectXGraphics::GetInstance()->SetUpContext(m_vShaderName, m_pShaderName);

			UINT strides = sizeof(CustomVertex);
			UINT offsets = 0;
			context->IASetInputLayout(m_inputLayout);
			context->IASetIndexBuffer(m_indexBufferList.at(count), DXGI_FORMAT_R32_UINT, 0);
			context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &strides, &offsets);
			context->VSSetConstantBuffers(0, 1, &m_constantBuffer);
			context->PSSetConstantBuffers(0, 1, &m_constantBuffer);

			context->DrawIndexed(index.second.size(), 0, 0);
			count++;
		}
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
				VecesData vertex = Analyze3Data(ss.str());
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
				NomalData normal = Analyze3Data(ss.str());
				objData.nomalDataList.push_back(normal);
			}
			else if (key.compare("f") == 0)
			{
				std::string m_data{};

				int vertexCount{ 0 };
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
							m_isAttachedTexture = false;
							count++;
							continue;
						}

						element[count] = std::stoi(num) - 1;
						count++;
					}

					CustomVertex customVertex{};

					int vertexNum = element.at(0);
					customVertex.vecesData = objData.vecesDataList.at(vertexNum);

					if (m_isAttachedTexture)
					{
						int texNum = element.at(1);
						customVertex.texData = objData.texDataList.at(texNum);
					}

					int normalNum = element.at(2);
					customVertex.nomalData = objData.nomalDataList.at(normalNum);

					custom_vertex_.push_back(customVertex);

					m_mtlIndex.at(currentMtlName).push_back((UINT)custom_vertex_.size() - 1);

					vertexCount++;
				}

				if (vertexCount == 4)
				{
					int indexListSize = m_mtlIndex.at(currentMtlName).size();
					m_mtlIndex.at(currentMtlName).push_back(m_mtlIndex.at(currentMtlName).at(indexListSize - 4));
					m_mtlIndex.at(currentMtlName).push_back(m_mtlIndex.at(currentMtlName).at(indexListSize - 2));
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

				std::vector<UINT> index{};
				m_mtlIndex.emplace(name, index);
			}
		}

		return true;
	}

	std::array<float, 3> Obj::Analyze3Data(std::string str_)
	{
		std::stringstream ss{ str_ };
		std::array<float, 3> result{};
		std::string num{};
		int count{ 0 };
		std::getline(ss, num, ' ');
		while (std::getline(ss, num, ' '))
		{
			result.at(count) = std::stof(num);
			count++;
		}

		return result;
	}

	std::array<float, 2> Obj::AnalyzeVTKey(std::stringstream ss)
	{
		std::array<float, 2> uv{};
		std::string num{};

		std::getline(ss, num, ' ');
		uv.at(0) = std::stof(num);

		std::getline(ss, num, ' ');
		uv.at(1) = std::stof(num) * -1;

		return uv;
	}

	void Obj::AnalyzeVNKey()
	{
	}

	void Obj::AnalyzeFKey()
	{
	}

	bool Obj::AnalyzeMtlData(std::string file_name_)
	{
		if (m_mtlFileName == "none") { return true; }

		char slash[] = "/";
		while (file_name_.back() != slash[0])
		{
			file_name_.erase(file_name_.size() - 1, 1);
		}
		file_name_ += m_mtlFileName;

		std::ifstream file(file_name_);
		if (!file)
		{
			m_isAttachedMtl = false;
			return true;
		}

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
				std::array<float, 3 > param = Analyze3Data(ss.str());
				m_mtlDataList.at(currentMtlName).Kd = param;
			}
			else if (key.compare("Ka") == 0)
			{
				std::array<float, 3 > param = Analyze3Data(ss.str());
				m_mtlDataList.at(currentMtlName).Ka = param;
			}
			else if (key.compare("newmtl") == 0)
			{
				std::string name{};
				std::getline(ss, name, ' ');

				currentMtlName = name;
				MtlDate mtlData{};
				m_mtlDataList.emplace(currentMtlName, mtlData);
			}
		}

		return true;
	}

	void Obj::Relese()
	{
		if (m_vertexBuffer != nullptr) { m_vertexBuffer->Release(); }
		if (m_constantBuffer != nullptr) { m_constantBuffer->Release(); }
		if (m_inputLayout != nullptr) { m_inputLayout->Release(); }
	}
}