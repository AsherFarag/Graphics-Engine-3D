#include "ResourceManager.h"



ResourceManager::ResourceManager()
{
	// TEMP
	// Load Phong Shader
	std::string FileName = std::string("./bin/shaders/Phong");
	std::string Vert = FileName.c_str();
	std::string Frag = FileName.c_str();

	m_PhongShader.loadShader(aie::eShaderStage::VERTEX,
		Vert.append(".vert").c_str());
	m_PhongShader.loadShader(aie::eShaderStage::FRAGMENT,
		Frag.append(".frag").c_str());
	if (m_PhongShader.link() == false)
	{
		printf(std::string("Phong").append(" Shader Error: %s\n").c_str(), m_PhongShader.getLastError());
	}
}

ResourceManager::~ResourceManager()
{
	while (m_LoadedMeshes.begin() != m_LoadedMeshes.end())
	{
		delete m_LoadedMeshes.begin()->second;
		m_LoadedMeshes.erase(m_LoadedMeshes.begin());
	}

	while (m_LoadedMaterials.begin() != m_LoadedMaterials.end())
	{
		delete m_LoadedMaterials.begin()->second;
		m_LoadedMaterials.erase(m_LoadedMaterials.begin());
	}
}

#pragma region Mesh Resources

ResourceManager* ResourceManager::GetInstance()
{
	if (s_ResourceInstance == nullptr)
		s_ResourceInstance = new ResourceManager();

	return s_ResourceInstance;
}

OBJMesh* ResourceManager::LoadMesh(const std::string& a_MeshName, RMaterial* a_Material, bool a_LoadTextures, bool a_FlipTexturesV)
{
	// If the mesh is already loaded, then return that
	auto* Mesh = GetLoadedMesh(a_MeshName);
	if (Mesh)
		return Mesh;

	// Construct new OBJMesh
	OBJMesh* MeshToLoad = new OBJMesh();

	if (MeshToLoad->load(a_MeshName.c_str(), a_LoadTextures, a_FlipTexturesV, a_Material));
	{
		GetInstance()->m_LoadedMeshes.insert({ a_MeshName , MeshToLoad });
		return MeshToLoad;
	}

	// If unable to load .obj
	delete MeshToLoad;
	return nullptr;
}

OBJMesh* ResourceManager::GetLoadedMesh(const std::string& a_MeshName)
{
	ResourceManager* Instance = GetInstance();

	auto LoadedMesh = Instance->m_LoadedMeshes.find(a_MeshName);

	// If the mesh is loaded
	if (LoadedMesh != Instance->m_LoadedMeshes.end())
		return LoadedMesh->second;

	return nullptr;
}

RMaterial* ResourceManager::InstantiateMaterial(const string& a_MaterialName)
{
	auto* Material = GetMaterial(a_MaterialName);
	if (Material == nullptr)
	{
		Material = new RMaterial();
		Material->m_Shader = &GetInstance()->m_PhongShader;
		GetInstance()->m_LoadedMaterials.insert({ a_MaterialName , Material });
	}

	return Material;
}

RMaterial* ResourceManager::GetMaterial(const string& a_MaterialName)
{
	auto Material = GetInstance()->m_LoadedMaterials.find(a_MaterialName);
	return Material != GetInstance()->m_LoadedMaterials.end() ? Material->second : nullptr;
}

#pragma endregion
