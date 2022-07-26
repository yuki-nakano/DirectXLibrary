#include "ObjManager.h"

namespace engine
{
	ObjManager::~ObjManager()
	{
		for (auto obj : m_objList)
		{
			delete obj.second;
		}
	}

	bool ObjManager::LoadObj(const std::string& file_name_, const std::string& name_)
	{
		Obj* obj = new Obj();

		if (!obj->Load(file_name_)) { return false; }

		m_objList.emplace(name_, obj);

		return true;
	}

	void ObjManager::DeleteObj(const std::string& name_)
	{
		delete m_objList.at(name_);

		m_objList.erase(name_);
	}

	void ObjManager::RenderObj(const std::string& name_, Vec3f pos_, Vec3f rote_, Vec3f scale_)
	{
		m_objList.at(name_)->Render(pos_, rote_, scale_);
	}

}