﻿#ifndef DIRECTX_TEXTURE_H
#define DIRECTX_TEXTURE_H

#include <string>
#include <map>
#include <d3d11.h>
#include <DirectXMath.h>

#include "../DirectX/DirectXUtility.h"
#include "../Utility/Singleton.h"

namespace engine
{
	/**
	* @brief テクスチャ描画クラス
	*/
	class DirectXTexture : public Singleton<DirectXTexture>
	{
	private:
		struct ConstantBuffer
		{
			DirectX::XMFLOAT4X4 world;	/// ワールド変換行列
			DirectX::XMFLOAT4 viewport;	/// ビューポート行列
		};

		struct CustomVertex
		{
			float pos[3];	/// 座標
			float uv[2];	/// uv値
		};

	private:
		friend class Singleton<DirectXTexture>;

		DirectXTexture() = default;
		~DirectXTexture() = default;

	public:
		/**
		* @brief 初期化
		* @return 初期化成功時true
		*/
		bool Init();

		/**
		* @brief 解放
		*/
		void Relese();

		/**
		* @brief テクスチャの読み込み
		* @param file_name_ 読み込むテクスチャファイルパス
		* @param name_　テクスチャ呼び出し時の名前
		* @return 読み込み成功時true
		*/
		bool LoadTexture(const std::wstring& file_name_, const std::string& name_);

		/**
		* @brief
		* @param name_ LoadTexture関数で登録した名前
		* @param pos_x_ x座標
		* @param pos_y_ y座標
		* @param widht_ 幅
		* @param height_ 高さ
		* @param angle_ 角度
		*/
		void DrawTexture(const std::string& name_, float pos_x_, float pos_y_, float width_, float height_, float angle_ = 0.0f);

		/**
		* @brief テクスチャの解放
		* @param name_ LoadTexture関数で登録した名前
		*/
		void ReleseTexture(const std::string& name_);

	private:
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

		/**
		* @brief Samplerの作成
		* @return 作成成功時true
		*/
		bool CreateSampler();

	private:
		/// 読み込んだテクスチャデータ保存
		std::map<std::string, ID3D11ShaderResourceView*> texList{};

		std::string m_vShaderName{ "VertexTex" };	/// vertexシェーダーを呼び出す用の名前
		std::string m_pShaderName{ "PixelTex" };	/// pixelシェーダーを呼び出す用の名前

		ID3D11Buffer* m_vertexBuffer;
		ID3D11Buffer* m_indexBuffer;
		ID3D11InputLayout* m_inputLayout;
		ID3D11Buffer* m_constantBuffer;
		ID3D11SamplerState* m_sampler;
	};
}

#endif // !DIRECTX_TEXTURE_H
