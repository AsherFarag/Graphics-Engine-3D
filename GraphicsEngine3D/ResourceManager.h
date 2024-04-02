#pragma once

// --- STD ---
#include <string>
#include <map>
using std::string;
using std::map;

// --- AssImp ---
#include "assimp/Importer.hpp"

// --- Engine ---
#include "OBJMesh.h"
#include "RMaterial.h"

#include "Shader.h"

class ResourceManager
{
protected:
	ResourceManager();
	~ResourceManager();

	// Allow access to destructor
	friend class GraphicsEngine3DApp;

	//  Type  , Map< Name,	Resource >
	map<size_t, map<size_t, RResource*>> m_LoadedResources;

public:
	ResourceManager(ResourceManager& Other) = delete;
	void operator=(const ResourceManager&) = delete;

	static ResourceManager* GetInstance();

#pragma region Mesh Resources

private:
	map<string, OBJMesh*> m_LoadedOBJMeshes;

public:
	static auto& GetLoadedOBJMeshes() { return GetInstance()->m_LoadedOBJMeshes; }
	static OBJMesh* LoadOBJMesh(const string& a_MeshName, RMaterial* a_Material = nullptr, bool a_LoadTextures = true, bool a_FlipTexturesV = false);
	static OBJMesh* GetLoadedOBJMesh(const string& a_MeshName);

	//map<string, RMesh*> m_LoadedMeshes;

public:
	//static auto& GetLoadedMeshes() { return GetInstance()->m_LoadedMeshes; }
	//static RMesh* LoadMesh(const string& a_MeshName, RMaterial* a_Material = nullptr, bool a_LoadTextures = true, bool a_FlipTexturesV = false);
	//static RMesh* GetLoadedMesh(const string& a_MeshName);

#pragma endregion

#pragma region Material Resources

	map<string, RMaterial*> m_LoadedMaterials;

public:
	static auto& GetLoadedMaterials() { return GetInstance()->m_LoadedMaterials; }
	//static RMaterial* LoadMaterial(const string& a_MaterialName, bool a_LoadTextures = true, bool a_FlipTexturesV = false);
	static RMaterial* InstantiateMaterial(const string& a_MaterialName, aie::ShaderProgram* a_ShaderProgram = nullptr);
	static RMaterial* GetMaterial(const string& a_MaterialName);

#pragma endregion

#pragma region Shaders

protected:
	aie::ShaderProgram* m_MainShader;

	map<string, ShaderProgram*> m_LoadedShaders;

public:
	static void ReloadShaders();
	static ShaderProgram* LoadShader(const string& a_FileName);
	static ShaderProgram* GetShader(const string& a_ShaderName);

	static void SetMainShader(ShaderProgram* a_Shader) { GetInstance()->m_MainShader = a_Shader; }

#pragma endregion


#pragma region Unimplemented
	template < typename T >
	static T* GetResource(const string& a_ResourceName)
	{
		auto& Type = typeid(T);
		auto TypeHash = Type.hash_code();

		ResourceManager* Instance = GetInstance();

		// Attempt to find a map of resource type T
		auto MapIterator = Instance->m_LoadedResources.find(TypeHash);
		if (MapIterator == Instance->m_LoadedResources.end())
			return nullptr;

		// Attempt to find the resource with a hash of a_ResourceName
		auto ResourceNameHash = std::hash<string>{}(a_ResourceName);
		auto ResourceIterrator = MapIterator->second.find(ResourceNameHash);
		if (ResourceIterrator == MapIterator->second.end())
			return nullptr;

		return (T*)ResourceIterrator->second;
	}

	template < typename T >
	static T* LoadResource(const string& a_ResourceName)
	{
		// If the resource is already loaded, then return it
		T* FoundResource = GetResource<T>();
		if (FoundResource)
			return FoundResource;


	}
#pragma endregion

};

static ResourceManager* s_ResourceInstance;

