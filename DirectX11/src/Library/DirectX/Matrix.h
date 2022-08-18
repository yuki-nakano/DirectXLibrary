#ifndef MATRIX_H
#define MATRIX_H

#include <DirectXMath.h>

#include "DirectXUtility.h"
#include "../Utility/Singleton.h"

namespace engine
{
	/**
	* @brief 行列生成クラス
	*/
	class Matrix : public Singleton<Matrix>
	{
	private:
		friend class Singleton<Matrix>;

		Matrix() = default;
		~Matrix() = default;

	public:
		/**
		* @breif ワールド座標変換行列の生成
		* @param pos_ 移動量
		* @param rote_ 角度
		* @param scale_ 大きさ
		* @retun ワールド座標変換行列
		*/
		DirectX::XMMATRIX CreateWorldMatrix(Vec3f pos_, Vec3f rote_, Vec3f scale_);

		/**
		* @breif 2D用のワールド座標変換行列の生成
		* @param pos_ 座標
		* @param width_ 横幅
		* @param height_ 縦幅
		* @param degree_ 角度
		* @retun ワールド座標変換行列
		*/
		DirectX::XMMATRIX CreateWorldMatrix2D(Vec2f pos_, float width_, float height_, float degree_);

		/**
		* @breif ビュー座標変換行列の生成
		* @retun ビュー座標変換行列
		*/
		DirectX::XMMATRIX CreateViewMatrix();

		/**
		* @breif プロジェクション座標変換行列の生成
		* @retun プロジェクション座標変換行列
		*/
		DirectX::XMMATRIX CreateProjMatrix();

		/**
		* @brief ビューポート用行列の取得
		*/
		DirectX::XMFLOAT4 GetViewportMatrix();

		/**
		* @brief ライトの座標行列の生成
		* @return ライト座標の行列
		*/
		DirectX::XMVECTOR CreateLightPosMatrix();

		/**
		* @brief ライトの色行列の生成
		* @return ライトの色行列
		*/
		DirectX::XMFLOAT4 CreateLightColMatrix();

		// アクセサ

		void SetViewport(Vec2f viewport_) { viewport = viewport_; }

	private:
		Vec2f viewport{};	/// ビューポート
	};
}

#endif // !MATRIX_H
