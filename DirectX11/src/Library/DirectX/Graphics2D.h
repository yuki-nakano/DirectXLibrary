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
		* @param pos_x_ 描画するx座標
		* @param pos_y_ 描画するy座標
		* @param widht_	三角形の横の辺の長さ
		* @param height_ 三角形の縦の辺の長さ
		* @param degree_ 描画する三角形の角度
		* @note 左上の頂点が90°の三角形の描画
		*/
		void DrawPorigon(float pos_x_, float pos_y_, float width_, float height_, float degree_ = 0.0f);

		/**
		* @brief 矩形の描画
		* @param pos_x_ 描画する矩形の左上のx座標
		* @param pos_y_ 描画する矩形の左上のy座標
		* @param widht_ 描画する矩形の横幅
		* @param height_ 描画する矩形の縦幅
		* @param degree_ 描画する矩形の角度
		* @note 各頂点の角度が90°の矩形の描画
		*/
		void DrawRect(float pos_x_, float pos_y_, float width_, float height_, float degree_ = 0.0f);

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
