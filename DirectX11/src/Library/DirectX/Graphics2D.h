#ifndef GRAPHICS_2D_H
#define GRAPHICS_2D_H

#include <DirectXMath.h>

#include "../Shader/ShaderManager.h"
#include "../DirectX/Matrix.h"
#include "../Utility/Singleton.h" 

namespace engine
{
	/**
	* @brief ポリゴンの描画クラス
	*/
	class Graphics2D : public Singleton<Graphics2D>
	{
	private:
		struct ConstantBuffer
		{
			DirectX::XMFLOAT4X4 world;	/// ワールド変換行列
			DirectX::XMFLOAT4 viewport;	/// ビューポート変換行列
		};

		struct CustomVertex
		{
			float pos[4];	/// 座標
			float col[4];	/// 色
		};

	private:
		friend class Singleton<Graphics2D>;

		Graphics2D() = default;
		~Graphics2D();

	public:
		/**
		* @brief 三角形の描画
		* @param pos_ 左上の座標
		* @param widht_ 矩形の横幅
		* @param height_ 矩形の縦幅
		* @param alpha_ 透過率(アルファ値)
		* @param degree_ 描画する三角形の角度
		* @note 各頂点の角度が90°の三角形の描画
		*/
		void DrawPorigon(const Vec2f& pos_, const float& width_, const float& height_, const Vec3f& color_, const float& alpha_ = 1.0f, const float& degree_ = 0.0f);

		/**
		* @brief 矩形の描画
		* @param pos_ 左上の座標
		* @param widht_ 矩形の横幅
		* @param height_ 矩形の縦幅
		* @param alpha_ 透過率(アルファ値)
		* @param degree_ 描画する矩形の角度
		* @note 各頂点の角度が90°の矩形の描画
		*/
		void DrawRect(const Vec2f& pos_, const float& width_, const float& height_, const Vec3f& color_, const float& alpha_ = 1.0f, const float& degree_ = 0.0f);

		/**
		* @brief VertexShader等の作成
		*/
		void Init();

	private:
		/**
		* @brief DrawRect関数用の情報生成
		*/
		void CreateRect();

		/**
		* @brief 作成したbuffer等の解放
		*/
		void Relese();

		/**
		* @brief bufferの更新
		* @param graphicsのコンテキスト
		*/
		void SetUpBuffers(ID3D11DeviceContext* context_, const Vec2f& pos_, const float& width_, const float& height_, const Vec3f& color_, const float& alpha_ = 1.0f, const float& degree_ = 0.0f);

		/**
		* @brief GPUへ送るデータの設定
		* @param graphicsのコンテキスト
		*/
		void SetUpContext(ID3D11DeviceContext* context_);

	private:
		std::string m_vShaderName{ "Vertex2D" };	/// vertexシェーダーを呼び出す用の名前
		std::string m_pShaderName{ "PixelBase" };	/// pixelシェーダーを呼び出す用の名前

		ID3D11InputLayout* m_inputLayout{ nullptr };
		ID3D11Buffer* m_indexBuffer{ nullptr };
		ID3D11Buffer* m_vertexBuffer{ nullptr };
		ID3D11Buffer* m_constantBuffer{ nullptr };

	};
}

#endif // !DIRECTX_GRAPHICS_2D_H
