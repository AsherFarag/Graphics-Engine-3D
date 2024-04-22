#pragma once
#include "RMesh.h"
#include "RSkeleton.h"

#include <map>

class MeshLoader
{
private:
	MeshLoader();
	~MeshLoader() = default;

public:
	MeshLoader( MeshLoader& Other ) = delete;
	void operator=( const MeshLoader& ) = delete;

	static MeshLoader* GetInstance();

	std::map<string, MeshHandle> m_LoadedMeshes;
	std::map<string, SkeletonHandle> m_LoadedSkeletons;

	MeshHandle LoadMesh( const string& a_Path, const string& a_Name, const aiScene* a_Scene );
	MeshHandle LoadMesh( const string& a_Path, bool a_GenerateMaterials );
	MeshHandle GetMesh( const string& a_Name );

	SkeletonHandle LoadSkeleton( const string& a_Path, const string& a_Name, const aiNode* a_BoneRootNode );
};