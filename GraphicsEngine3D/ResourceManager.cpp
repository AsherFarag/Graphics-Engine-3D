#include "ResourceManager.h"

// --- AssImp ---
#include "assimp/postprocess.h"
#include "assimp/scene.h"

#include "RMesh.h"


//ResourceManager::ResourceManager()
//{
//}
//
//ResourceManager::~ResourceManager()
//{
//	while (m_LoadedOBJMeshes.begin() != m_LoadedOBJMeshes.end())
//	{
//		delete m_LoadedOBJMeshes.begin()->second;
//		m_LoadedOBJMeshes.erase(m_LoadedOBJMeshes.begin());
//	}
//
//	while (m_LoadedMaterials.begin() != m_LoadedMaterials.end())
//	{
//		delete m_LoadedMaterials.begin()->second;
//		m_LoadedMaterials.erase(m_LoadedMaterials.begin());
//	}
//
//	while (m_LoadedShaders.begin() != m_LoadedShaders.end())
//	{
//		delete m_LoadedShaders.begin()->second;
//		m_LoadedShaders.erase(m_LoadedShaders.begin());
//	}
//}
//
//#pragma region Mesh Resources
//
//ResourceManager* ResourceManager::GetInstance()
//{
//	if (s_ResourceInstance == nullptr)
//		s_ResourceInstance = new ResourceManager();
//
//	return s_ResourceInstance;
//}
//
//OBJMesh* ResourceManager::LoadOBJMesh(const std::string& a_MeshName, RMaterial* a_Material, bool a_LoadTextures, bool a_FlipTexturesV)
//{
//	// If the mesh is already loaded, then return that
//	auto* Mesh = GetLoadedOBJMesh(a_MeshName);
//	if (Mesh)
//		return Mesh;
//
//	// Construct new OBJMesh
//	OBJMesh* MeshToLoad = new OBJMesh();
//
//	if (MeshToLoad->load(a_MeshName.c_str(), a_LoadTextures, a_FlipTexturesV, a_Material));
//	{
//		GetInstance()->m_LoadedOBJMeshes.insert({ a_MeshName , MeshToLoad });
//		return MeshToLoad;
//	}
//
//	// If unable to load .obj
//	delete MeshToLoad;
//	return nullptr;
//}
//
//OBJMesh* ResourceManager::GetLoadedOBJMesh(const std::string& a_MeshName)
//{
//	ResourceManager* Instance = GetInstance();
//
//	auto LoadedMesh = Instance->m_LoadedOBJMeshes.find(a_MeshName);
//
//	// If the mesh is loaded
//	if (LoadedMesh != Instance->m_LoadedOBJMeshes.end())
//		return LoadedMesh->second;
//
//	return nullptr;
//}
//
////RMesh* ResourceManager::LoadMesh(const string& a_MeshName, RMaterial* a_Material, bool a_LoadTextures, bool a_FlipTexturesV)
////{
////	ResourceManager* Instance = GetInstance();
////
////	auto Mesh = new RMesh();
////
////	// === Step 1 ===
////	// Construct the full file path with file name
////
////	// Set File type extension
////	Mesh->m_FileType = ".obj";
////
////	// Construct the file path
////	Mesh->m_FilePath = string(RESOURCE_PATH) + MESH_FILE_PATH + a_MeshName;
////
////	// Get the name of the File
////	Mesh->m_ResourceName = Mesh->m_FilePath.substr(Mesh->m_FilePath.find_last_of('/') + 1);
////
////	// Attach file extension
////	Mesh->m_FilePath.append(Mesh->m_FileType);
////
////
////	Assimp::Importer Importer;
////	const aiScene* Model = Importer.ReadFile(Mesh->m_FilePath, aiProcess_JoinIdenticalVertices | aiProcess_Triangulate);
////
////	const auto AssimpMesh = Model->mMeshes[0];
////
////	return nullptr;
////}
////
////RMesh* ResourceManager::GetLoadedMesh(const string& a_MeshName)
////{
////	ResourceManager* Instance = GetInstance();
////	return nullptr;
////}
//
//RMaterial* ResourceManager::InstantiateMaterial(const string& a_MaterialName, aie::ShaderProgram* a_ShaderProgram)
//{
//	auto* Material = GetMaterial(a_MaterialName);
//	if (Material == nullptr)
//	{
//		Material = new RMaterial();
//
//		GetInstance()->m_LoadedMaterials.insert({ a_MaterialName , Material });
//	}
//
//	a_ShaderProgram == nullptr ? Material->m_Shader = GetInstance()->m_MainShader : a_ShaderProgram;
//
//	return Material;
//}
//
//RMaterial* ResourceManager::GetMaterial(const string& a_MaterialName)
//{
//	auto Material = GetInstance()->m_LoadedMaterials.find(a_MaterialName);
//	return Material != GetInstance()->m_LoadedMaterials.end() ? Material->second : nullptr;
//}
//
//void ResourceManager::ReloadShaders()
//{
//	string FilePath = "Resources/Shaders/";
//
//	auto& LoadedShaders = GetInstance()->m_LoadedShaders;
//	for (auto i = LoadedShaders.begin(); i != LoadedShaders.end(); i++)
//	{
//		i->second->loadShader(aie::eShaderStage::VERTEX, (FilePath + i->first + ".vert").c_str());
//		i->second->loadShader(aie::eShaderStage::FRAGMENT, (FilePath + i->first + ".frag").c_str());
//	}
//}
//
//ShaderProgram* ResourceManager::LoadShader(const string& a_FileName)
//{
//	string FilePath = "Resources/Shaders/";
//
//	auto* Shader = GetShader(a_FileName);
//	if (Shader == nullptr)
//	{
//		Shader = new aie::ShaderProgram();
//		Shader->loadShader(aie::eShaderStage::VERTEX, (FilePath + a_FileName + ".vert").c_str());
//		Shader->loadShader(aie::eShaderStage::FRAGMENT, (FilePath + a_FileName + ".frag").c_str());
//		GetInstance()->m_LoadedShaders.insert({ a_FileName , Shader });
//	}
//
//	if (Shader->link() == false)
//	{
//		printf((a_FileName + " Shader Error: %s\n").c_str(), Shader->getLastError());
//		return Shader;
//	}
//
//	return Shader;
//}
//
//ShaderProgram* ResourceManager::GetShader(const string& a_ShaderName)
//{
//	auto Shader = GetInstance()->m_LoadedShaders.find(a_ShaderName);
//	return Shader != GetInstance()->m_LoadedShaders.end() ? Shader->second : nullptr;
//}
//
//#pragma endregion