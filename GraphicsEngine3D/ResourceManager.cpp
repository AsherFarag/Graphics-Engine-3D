#include "ResourceManager.h"



ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
}

ResourceManager* ResourceManager::GetInstance()
{
	if (s_ResourceInstance == nullptr)
		s_ResourceInstance = new ResourceManager();

	return s_ResourceInstance;
}

OBJMesh* ResourceManager::LoadMesh(const std::string& a_MeshName, bool a_LoadTextures, bool a_FlipTexturesV)
{
	OBJMesh* MeshToLoad = new OBJMesh();
	if (MeshToLoad->load(a_MeshName.c_str(), a_LoadTextures, a_FlipTexturesV));
	{
		GetInstance()->m_LoadedMeshes.insert({ a_MeshName , MeshToLoad });
		return MeshToLoad;
	}

	delete MeshToLoad;
	return nullptr;
}

OBJMesh* ResourceManager::GetLoadedMesh(const std::string& a_MeshName, bool a_LoadTextures, bool a_FlipTexturesV)
{
	ResourceManager* Instance = GetInstance();

	auto LoadedMesh = Instance->m_LoadedMeshes.find(a_MeshName);

	// If the mesh is loaded
	if (LoadedMesh != Instance->m_LoadedMeshes.end())
		return LoadedMesh->second;

	// Else Load the Mesh
	return LoadMesh(a_MeshName, a_LoadTextures, a_FlipTexturesV);
}
