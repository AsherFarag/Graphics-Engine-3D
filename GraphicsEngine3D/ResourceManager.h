#pragma once

// --- STD ---
#include <string>
#include <map>
using std::string;
using std::map;

// --- Engine ---
#include "OBJMesh.h"

class ResourceManager
{
protected:
	ResourceManager();
	~ResourceManager();

	//  Type  , Map< Name,	Resource >
	map<size_t, map<size_t, RResource*>> m_LoadedResources;

	map<string, OBJMesh*> m_LoadedMeshes;

public:
	ResourceManager(ResourceManager& Other) = delete;
	void operator=(const ResourceManager&) = delete;

	static ResourceManager* GetInstance();

	static map<string, OBJMesh*>& GetLoadedMeshes() { return GetInstance()->m_LoadedMeshes; }
	static OBJMesh* LoadMesh(const string& a_MeshName, bool a_LoadTextures = true, bool a_FlipTexturesV = false);
	static OBJMesh* GetLoadedMesh(const string& a_MeshName, bool a_LoadTextures = true, bool a_FlipTexturesV = false);

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
};

static ResourceManager* s_ResourceInstance;

