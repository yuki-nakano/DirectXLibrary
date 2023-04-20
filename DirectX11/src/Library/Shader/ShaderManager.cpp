#include "ShaderManager.h"

namespace engine
{
	ShaderManager::~ShaderManager()
	{
		for (auto shader : m_shaderList)
		{
			shader.second->DeleteShader();
		}
	}

	bool ShaderManager::Init()
	{
		if (!CreateShader("src/Library/cso/PixelShader.cso", "PixelBase", ShaderType::Pixel)) { return false; }
		if (!CreateShader("src/Library/cso/VertexShader.cso", "VertexBase", ShaderType::Vertex)) { return false; }

		// 2Dポリゴン描画用
		if (!CreateShader("src/Library/cso/Vertex2D.cso", "Vertex2D", ShaderType::Vertex)) { return false; }

		// obj用
		if (!CreateShader("src/Library/cso/PixelObj.cso", "PixelObj", ShaderType::Pixel)) { return false; }
		if (!CreateShader("src/Library/cso/VertexObj.cso", "VertexObj", ShaderType::Vertex)) { return false; }

		// テクスチャ用
		if (!CreateShader("src/Library/cso/VertexTex.cso", "VertexTex", ShaderType::Vertex)) { return false; }

		// テクスチャ用
		if (!CreateShader("src/Library/cso/PixcelTex.cso", "PixelTex", ShaderType::Pixel)) { return false; }

		return true;
	}

	bool ShaderManager::CreateShader(const std::string& file_name_,
		const std::string& shader_name_, const ShaderType& shader_type_)
	{
		ShaderBase* shader{ nullptr };

		switch (shader_type_)
		{
		case engine::ShaderType::Pixel:
			shader = new PixelShader();
			break;
		case engine::ShaderType::Vertex:
			shader = new VertexShader();
			break;
		default:
			return false;
			break;
		}

		if (!shader->LoadFile(file_name_)) { return false; }
		if (!shader->CreateShader()) { return false; }

		m_shaderList.try_emplace(shader_name_, shader);

		return true;
	}

	void ShaderManager::DeleteShader(const std::string& shader_name_)
	{
		m_shaderList.at(shader_name_)->DeleteShader();
		m_shaderList.erase(shader_name_);
	}

	long ShaderManager::GetShaderSize(const std::string& shader_name_)
	{
		return m_shaderList.at(shader_name_)->GetSize();
	}

	char* ShaderManager::GetShaderData(const std::string& shader_name_)
	{
		return m_shaderList.at(shader_name_)->GetData();
	}

	ID3D11VertexShader* ShaderManager::GetVertexInterface(const std::string& shader_name_)
	{
		return m_shaderList.at(shader_name_)->GetVertexInterface();
	}

	ID3D11PixelShader* ShaderManager::GetPixelInterface(const std::string& shader_name_)
	{
		return m_shaderList.at(shader_name_)->GetPixelInterface();
	}
}
