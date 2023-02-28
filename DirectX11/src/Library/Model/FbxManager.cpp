#include "FbxManager.h"


namespace engine
{
	FBXManager::~FBXManager()
	{
		for (auto fbx : m_fbxList)
		{
			delete fbx.second;
		}

		m_fbxManager->Destroy();
	}

	bool FBXManager::Init()
	{
		m_fbxManager = FbxManager::Create();

		if (!m_fbxManager) { return false; }

		return true;
	}

	bool FBXManager::LoadFbx(const std::string& file_name_, const std::string& name_)
	{
		Fbx* fbx = new Fbx(m_fbxManager);

		if (!fbx->Load(file_name_)) { return false; }

		m_fbxList.try_emplace(name_, fbx);

		return true;
	}

	void FBXManager::DeleteFbx(const std::string& name_)
	{
		delete m_fbxList.at(name_);

		m_fbxList.erase(name_);
	}

	void FBXManager::Render(const std::string& name_, Vec3f pos_, Vec3f rote_, Vec3f scale_)
	{
		//m_fbxList.at(name_)->Render(pos_, rote_, scale_);

		i++;
		m_fbxList.at(name_)->RenderAnimation(pos_, rote_, scale_, i);
	}
}