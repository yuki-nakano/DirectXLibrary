#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include <string>
#include <map>
#include <d3d11.h>
#include <DirectXMath.h>

#include "ShaderBase.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "../Utility/Singleton.h"

namespace engine
{
	/**
	* @brief シェーダー管理クラス
	*/
	class ShaderManager : public Singleton<ShaderManager>
	{
	private:
		friend class Singleton<ShaderManager>;

		ShaderManager() = default;
		~ShaderManager();

	public:
		/**
		* @brief 初期シェーダー生成
		* @return シェーダー作成成功時true
		*/
		bool Init();

		/**
		* @brief シェーダーの作成
		* @param file_name_ 読み込むシェーダーファイルパス
		* @param shader_name_ シェーダー呼び出し時の名前
		* @param shader_type_ シェーダーのタイプ
		* @return シェーダー作成成功時true
		*/
		bool CreateShader(const std::string& file_name_, const std::string& shader_name_, const ShaderType& shader_type_);

		/**
		* @brief シェーダーの削除
		* @param shader_name_ CreateShader関数で登録した名前
		*/
		void DeleteShader(const std::string& shader_name_);

		/**
		* @brief シェーダーサイズの取得
		* @param shader_name_ CreateShader関数で登録した名前
		* @return シェーダーサイズ
		*/
		long GetShaderSize(const std::string& shader_name_);

		/**
		* @brief シェーダー情報の取得
		* @param shader_name_ CreateShader関数で登録した名前
		* @return シェーダー情報
		*/
		char* GetShaderData(const std::string& shader_name_);

		/**
		* @breif VertexShaderのInterfaceの取得
		* @param shder_name_
		*/
		ID3D11VertexShader* GetVertexInterface(const std::string& shader_name_);

		/**
		* @breif PixelShaderのInterfaceの取得
		* @param shader_name_ CreateShader関数で登録した名前
		*/
		ID3D11PixelShader* GetPixelInterface(const std::string& shader_name_);

	private:
		/// シェーダー保存用
		std::map<std::string, ShaderBase*> m_shaderList{};
	};
}

#endif // !SHADER_MANAGER_H
