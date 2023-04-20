#ifndef FBX_H
#define FBX_H

#include <array>
#include <vector>
#include <map>
#include <string>
#include <d3d11.h>
#include <DirectXMath.h>
#include <fbxsdk.h>

#include "../DirectX/DirectXUtility.h"

namespace engine
{
	using VecesData = std::array<float, 3>;
	using TextureUV = std::array<float, 2>;
	using NomalData = std::array<float, 3>;
	using WeightData = std::map<int, float>; // <頂点インデックス, ウェイト>

	class Fbx
	{
	private:
		struct CustomVertex
		{
			VecesData vecesData;
			NomalData nomalData;
			TextureUV texData;
		};

		struct ConstantBuffer
		{
			DirectX::XMFLOAT4X4 world;	/// ワールド変換行列
			DirectX::XMFLOAT4X4 view;	/// ビューポート変換行列
			DirectX::XMFLOAT4X4 proj;	/// プロジェクション変換行列
			DirectX::XMFLOAT4 LightVec;	/// ライト座標行列
			DirectX::XMFLOAT4 LightCol;	/// ライト色行列
			DirectX::XMFLOAT4 mtlDiffuse;	/// ディフューズ
			DirectX::XMFLOAT4 mtlAmbient;	/// アンビエント
			DirectX::XMFLOAT4 col;		/// 色
		};

	public:
		Fbx(FbxManager* fbx_manager_);
		~Fbx();

	public:
		/**
		* @brief fbxファイルの読み込みファイルの読み込み
		* @param file_name_ 読み込むfbxファイルパス
		* @return fbx読み込み成功時true
		*/
		bool Load(const std::string& file_name_);

		/**
		* @brief fbx描画
		* @param fbx呼び出し時の名前
		* @param pos_ 移動量
		* @param rote_ 角度
		* @param scale_ 大きさ
		*/
		void Render(const Vec3f& pos_, const Vec3f& rote_, const Vec3f& scale_);

		/**
		* @brief アニメーションfbx描画
		* @param fbx呼び出し時の名前
		* @param pos_ 移動量
		* @param rote_ 角度
		* @param scale_ 大きさ
		* @param frame_ 描画するfbxアニメーションのフレーム数
		*/
		void RenderAnimation(const Vec3f& pos_, const Vec3f& rote_, const Vec3f& scale_, int frame_);

	private:
		/**
		* @brief ノードの検索
		* @param [in] node_  親ノード
		* @param [in] node_type_ 検索するノードのタイプ
		* @param [out] node_lidt_ 検索結果保存用
		*/
		void SearchNode(FbxNode* node_, FbxNodeAttribute::EType node_type_, std::map<std::string, FbxNode*>& node_list_);

	private:
		std::map<std::string, ID3D11Buffer*> m_vertexBufferList{};
		std::map<std::string, ID3D11Buffer*> m_indexBufferList{};
		ID3D11Buffer* m_constantBuffer{ nullptr };
		ID3D11InputLayout* m_inputLayout{ nullptr };

		std::map<std::string, std::vector<CustomVertex>> m_customVertexList{};
		std::map<std::string, std::vector<int>> m_index{};

		bool m_canAnimation{ true };

		FbxNode* m_rootNode{};
		std::map<std::string, FbxNode*> m_meshNodeList{};		/// メッシュノード
		std::map<std::string, FbxNode*> m_skeletonNodeList{};	/// スケルトンノード

		std::map<std::string, FbxMatrix> m_initPostureMatrix;	/// 初期姿勢行列

		std::map<std::string, WeightData> m_boneWeightList{};
		std::map<std::string, FbxMatrix > m_initMatrixList{};

		FbxManager* m_fbxManager{};
		FbxImporter* m_fbxImporter{};
		FbxScene* m_fbxScene{};

		std::string m_vShaderName{ "VertexObj" };	/// vertexシェーダーを呼び出す用の名前
		std::string m_pShaderName{ "PixelBase" };	/// pixelシェーダーを呼び出す用の名前
	};
}

#endif // !FBX_H
