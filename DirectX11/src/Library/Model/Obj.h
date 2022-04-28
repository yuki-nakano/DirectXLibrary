﻿#ifndef OBJ_H
#define OBJ_H

#include <array>
#include <vector>
#include <map>
#include <string>
#include <d3d11.h>
#include <DirectXMath.h>

#include "../DirectX/DirectXUtility.h"
#include "../Utility/Singleton.h"

namespace engine
{
	using VecesData = std::array<float, 3>;
	using NomalData = std::array<float, 3>;

	class Obj
	{
	private:
		/// 解析データ
		struct ObjData
		{
			std::vector<VecesData> vecesDataList;
			std::vector<NomalData> nomalDataList;
		};

		struct CustomVertex
		{
			VecesData vecesData;
			NomalData nomalData;
		};

		struct ConstantBuffer {
			DirectX::XMFLOAT4X4 world;
			DirectX::XMFLOAT4X4 view;
			DirectX::XMFLOAT4X4 proj;
			DirectX::XMFLOAT4 LightVec;
			DirectX::XMFLOAT4 LightCol;
		};

	public:
		Obj();
		~Obj();

	public:
		/**
		* @brief objファイルの読み込みファイルの読み込み
		* @param file_name_ 読み込むobjファイルパス
		* @return obj読み込み成功時true
		*/
		bool Load(const std::string& file_name_);

		void Relese();

		/**
		* @brief objモデルの描画
		* @param pos_ 移動量
		* @param rote_ 角度
		* @param scale_ 大きさ
		*/
		void Render(Vec3f pos_, Vec3f rote_, Vec3f scale_);

	private:
		/**
		* @brief objファイル解析
		* @param fiel_name_[in] 読み込むobjファイルパス
		* @param custom_vertex_[out] 解析データ保存用
		* @return 解析成功時true
		*/
		bool AnalyzeData(const std::string& file_name_, std::vector<CustomVertex>& custom_vertex_);

		/**
		* @brief VertexBufferの作成
		* @return 作成成功時true
		*/
		bool CreateVertexBuffer();

		/**
		* @brief IndexBufferの作成
		* @return 作成成功時true
		*/
		bool CreateIndexBuffer();

		/**
		* @brief InputLayoutの作成
		* @return 作成成功時true
		*/
		bool CreateInputLayout();

		/**
		* @brief ConstantBufferの作成
		* @return 作成成功時true
		*/
		bool CreateConstantBuffer();

	private:
		ID3D11Buffer* m_vertexBuffer{};
		ID3D11Buffer* m_indexBuffer{};
		ID3D11Buffer* m_constantBuffer{};
		ID3D11InputLayout* m_inputLayout{};
		int m_vertexNum;
		std::vector<UWORD> m_index;

		std::string m_vShaderName{ "VertexObj" };
		std::string m_pShaderName{ "PixelBase" };

	};
}

#endif // !OBJ_H