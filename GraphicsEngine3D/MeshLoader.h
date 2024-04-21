#pragma once
#include "RMesh.h"

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

	MeshHandle LoadMesh( const string& a_Path, bool a_GenerateMaterials );
	MeshHandle GetMesh( const string& a_Name );
};