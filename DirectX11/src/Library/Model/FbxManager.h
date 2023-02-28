#ifndef FBX_MANAGER_H
#define FBX_MANAGER_H

#include <map>
#include <string>
#include <fbxsdk.h>

#include "../DirectX/DirectXUtility.h"
#include "../Utility/Singleton.h"
#include "Fbx.h"

namespace engine
{
	/**
	* @brief obj情報管理
	*/
	class FBXManager : public Singleton<FBXManager>
	{
	private:
		friend class Singleton<FBXManager>;

		FBXManager() = default;
		~FBXManager();

	public:

		bool Init();

		/**
		* @brief fbxファイルの読み込みファイルの読み込み
		* @param file_name_ 読み込むfbxファイルパス
		* @param name_　fbx呼び出し時の名前
		* @return fbx読み込み成功時true
		*/
		bool LoadFbx(const std::string& file_name_, const std::string& name_);

		/**
		* @brief 読み込んだobjの削除
		* @param LoadFbx関数で登録した名前
		*/
		void DeleteFbx(const std::string& name_);

		/**
		* @brief fbx描画
		* @param fbx呼び出し時の名前
		* @param pos_ 移動量
		* @param rote_ 角度
		* @param scale_ 大きさ
		*/
		void Render(const std::string& name_, Vec3f pos_, Vec3f rote_, Vec3f scale_);

		///**
		//* @brief obj描画
		//* @param obj呼び出し時の名前
		//* @param pos_ 移動量
		//* @param rote_ 角度
		//* @param scale_ 大きさ
		//* @param color_ 色
		//* @param alpha_ 透過
		//*/
		//void RenderSetColor(const std::string& name_, const Vec3f& pos_, const Vec3f& rote_, const Vec3f& scale_, const Vec3f& color_, const float& alpha_ = 1.0f);

		//Obj* GetObj(const std::string& name_) { return m_objList.at(name_); }

	private:
		/// FBX情報保存
		std::map<std::string, Fbx*> m_fbxList;

		FbxManager* m_fbxManager{};

		int i{ 0 };
	};
}

#endif // !FBX_MANAGER_H
