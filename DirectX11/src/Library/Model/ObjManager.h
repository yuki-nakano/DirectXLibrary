#ifndef OBJ_MANAGER_H
#define OBJ_MANAGER_H

#include <map>
#include <string>

#include "Obj.h"
#include "../Utility/Singleton.h"

namespace engine
{
	/**
	* @brief obj情報管理
	*/
	class ObjManager : public Singleton<ObjManager>
	{
	private:
		friend class Singleton<ObjManager>;

		ObjManager() = default;
		~ObjManager();

	public:
		/**
		* @brief objファイルの読み込みファイルの読み込み
		* @param file_name_ 読み込むobjファイルパス
		* @param name_　obj呼び出し時の名前
		* @return obj読み込み成功時true
		*/
		bool LoadObj(const std::string& file_name_, const std::string& name_);

		/**
		* @brief 読み込んだobjの削除
		* @param LoadObj関数で登録した名前
		*/
		void DeleteObj(const std::string& name_);

		/**
		* @brief obj描画
		* @param obj呼び出し時の名前
		* @param pos_ 移動量
		* @param rote_ 角度
		* @param scale_ 大きさ
		*/
		void Render(const std::string& name_, Vec3f pos_, Vec3f rote_, Vec3f scale_);

		/**
		* @brief obj描画
		* @param obj呼び出し時の名前
		* @param pos_ 移動量
		* @param rote_ 角度
		* @param scale_ 大きさ
		* @param color_ 色
		* @param alpha_ 透過
		*/
		void RenderSetColor(const std::string& name_, const Vec3f& pos_, const Vec3f& rote_, const Vec3f& scale_, const Vec3f& color_, const float& alpha_ = 1.0f);

		Obj* GetObj(const std::string& name_) { return m_objList.at(name_); }

	private:
		/// obj情報保存
		std::map<std::string, Obj*> m_objList;
	};
}

#endif // !OBJ_MANAGER_H
