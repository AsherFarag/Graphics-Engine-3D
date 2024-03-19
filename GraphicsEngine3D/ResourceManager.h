#pragma once

// --- STD ---
#include <string>
#include <map>

// --- Engine ---
#include "OBJMesh.h"

class ResourceManager
{
protected:
	ResourceManager();
	~ResourceManager();

	std::map<std::string, OBJMesh*> m_LoadedMeshes;

public:
	ResourceManager(ResourceManager& Other) = delete;
	void operator=(const ResourceManager&) = delete;

	static ResourceManager* GetInstance();

	static OBJMesh* LoadMesh(const std::string& a_MeshName, bool a_LoadTextures = true, bool a_FlipTexturesV = false);
	static OBJMesh* GetLoadedMesh(const std::string& a_MeshName, bool a_LoadTextures = true, bool a_FlipTexturesV = false);
};

static ResourceManager* s_ResourceInstance;

