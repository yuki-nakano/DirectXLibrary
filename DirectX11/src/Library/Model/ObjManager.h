﻿#ifndef OBJ_MANAGER_H
#define OBJ_MANAGER_H

#include <map>
#include <string>

#include "Obj.h"
#include "../Utility/Singleton.h"

namespace engine
{
	class ObjManager : public Singleton<ObjManager>
	{
	private:
		friend class Singleton<ObjManager>;

		ObjManager();
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
		*/
		void RenderObj(const std::string& name_, Vec3f pos_, Vec3f rote_, Vec3f scale_);

	private:
		std::map<std::string, Obj*> m_objList;
	};
}

#endif // !OBJ_MANAGER_H
