#ifndef OBJ_H
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

	/**
	* @brief objファイルの読み込みと描画クラス
	*/
	class Obj
	{
	private:
		/// 解析データ
		struct ObjData
		{
			std::vector<VecesData> vecesDataList;	/// 頂点データ
			std::vector<NomalData> nomalDataList;	/// 法線データ
		};

		struct CustomVertex
		{
			VecesData vecesData;
			NomalData nomalData;
		};

		struct ConstantBuffer
		{
			DirectX::XMFLOAT4X4 world;	/// ワールド変換行列
			DirectX::XMFLOAT4X4 view;	/// ビューポート変換行列
			DirectX::XMFLOAT4X4 proj;	/// プロジェクション変換行列
			DirectX::XMFLOAT4 LightVec;	/// ライト座標行列
			DirectX::XMFLOAT4 LightCol;	/// ライト色行列
		};

	public:
		Obj() = default;
		~Obj();

	public:
		/**
		* @brief objファイルの読み込みファイルの読み込み
		* @param file_name_ 読み込むobjファイルパス
		* @return obj読み込み成功時true
		*/
		bool Load(const std::string& file_name_);

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
		* @brief 作成したbuffer等の解放
		*/
		void Relese();

	private:
		ID3D11Buffer* m_vertexBuffer{ nullptr };
		ID3D11Buffer* m_indexBuffer{ nullptr };
		ID3D11Buffer* m_constantBuffer{ nullptr };
		ID3D11InputLayout* m_inputLayout{ nullptr };
		int m_vertexNum;	/// 頂点数
		std::vector<UWORD> m_index{};

		std::string m_vShaderName{ "VertexObj" };	/// vertexシェーダーを呼び出す用の名前
		std::string m_pShaderName{ "PixelBase" };	/// pixelシェーダーを呼び出す用の名前

	};
}

#endif // !OBJ_H
