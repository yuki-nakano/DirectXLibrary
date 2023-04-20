#ifndef VERTEX_SHADER_H
#define VERTEX_SHADER_H

#include "ShaderBase.h"

namespace engine
{
	/**
	* @brief バーテックスシェーダー
	*/
	class VertexShader : public ShaderBase
	{
	public:
		VertexShader() = default;
		~VertexShader() = default;

	public:
		/**
		* @brief シェーダーの作成
		* @return 作成成功時にtrue
		*/
		bool CreateShader();

		/**
		* @brief シェーダの削除
		*/
		void DeleteShader();

		// アクセサ

		ID3D11VertexShader* GetVertexInterface() { return m_interface; }

	private:
		/// 作成されたシェーダー保存
		ID3D11VertexShader* m_interface;
	};
}

#endif // !VERTEX_SHADER_H
