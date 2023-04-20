#ifndef SHADER_BASE_H
#define SHADER_BASE_H

#include "../DirectX/DirectXGraphics.h"
#include <string>

namespace engine
{
	/// シェーダーのタイプ
	enum class ShaderType
	{
		Pixel,	/// ピクセルシェーダー
		Vertex	/// バーテックスシェーダー
	};

	/**
	* @brief シェーダー基底クラス
	*/
	class ShaderBase
	{
	public:
		ShaderBase() = default;
		~ShaderBase() = default;

	public:
		/**
		* @brief HLSLファイルの読み込み
		* @param file_name_ 読み込むシェーダーファイルパス
		*/
		bool LoadFile(const std::string& file_name_);

		virtual bool CreateShader() = 0;

		virtual void DeleteShader() = 0;

		// アクセサ

		long GetSize() { return m_size; }
		char* GetData() { return m_data; }

		virtual ID3D11VertexShader* GetVertexInterface() { return nullptr; }
		virtual ID3D11PixelShader* GetPixelInterface() { return nullptr; }

	protected:
		/// シェーダーサイズ
		long m_size{ 0 };
		/// シェーダー情報
		char* m_data;
	};
}

#endif // !SHADER_BASE_H	
