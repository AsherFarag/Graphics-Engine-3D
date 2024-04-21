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
#include "RTexture.h"

#include "Shader.h"

#include "MeshLoader.h"
#include "ShaderLoader.h"
#include "MaterialLoader.h"
#include "TextureLoader.h"

//class ResourceManager
//{
//private:
//	ResourceManager();
//	~ResourceManager();
//
//	// Allow access to destructor
//	friend class GraphicsEngine3DApp;
//
//public:
//	ResourceManager(ResourceManager& Other) = delete;
//	void operator=(const ResourceManager&) = delete;
//
//	static ResourceManager* GetInstance();
//
//private:
//	aie::ShaderProgram* m_MainShader;
//
//	map<string, OBJMesh*> m_LoadedOBJMeshes;
//	map<string, RMaterial*> m_LoadedMaterials;
//	map<string, ShaderProgram*> m_LoadedShaders;
//
//public:
//	static auto&	GetLoadedOBJMeshes() { return GetInstance()->m_LoadedOBJMeshes; }
//	static OBJMesh* LoadOBJMesh(const string& a_MeshName, RMaterial* a_Material = nullptr, bool a_LoadTextures = true, bool a_FlipTexturesV = false);
//	static OBJMesh* GetLoadedOBJMesh(const string& a_MeshName);
//
//	static auto& GetLoadedMaterials() { return GetInstance()->m_LoadedMaterials; }
//	static RMaterial* InstantiateMaterial( const string& a_MaterialName, aie::ShaderProgram* a_ShaderProgram = nullptr );
//	static RMaterial* GetMaterial( const string& a_MaterialName );
//
//	static void ReloadShaders();
//	static ShaderProgram* LoadShader( const string& a_FileName );
//	static ShaderProgram* GetShader( const string& a_ShaderName );
//
//	static void SetMainShader( ShaderProgram* a_Shader ) { GetInstance()->m_MainShader = a_Shader; }
//};
//
//static ResourceManager* s_ResourceInstance;

namespace Resource
{
	// - Meshes -
	static MeshHandle LoadMesh( const std::string& a_Path, bool a_GenerateMaterials ) { return MeshLoader::GetInstance()->LoadMesh( a_Path, a_GenerateMaterials ); }
	static MeshHandle GetMesh( const std::string& a_Name ) { return MeshLoader::GetInstance()->GetMesh( a_Name ); }

	// - Shaders -
	static ShaderHandle LoadShader( const std::string& a_Path ) { return ShaderLoader::GetInstance()->LoadShader( a_Path ); }
	static ShaderHandle GetShader( const std::string& a_Name ) { return ShaderLoader::GetInstance()->GetShader( a_Name ); }

	// - Materials -
	static MaterialHandle GetMaterial( const string& a_Name ) { return MaterialLoader::GetInstance()->GetMaterial( a_Name ); }
	static MaterialHandle InstantiateMaterial( const string& a_Name, ShaderHandle a_Shader = nullptr ) { return MaterialLoader::GetInstance()->InstantiateMaterial( a_Name, a_Shader ); }
	static MaterialInstanceHandle GetMaterialInstance( const string& a_Name ) { return MaterialLoader::GetInstance()->GetMaterialInstance( a_Name ); }
	static MaterialInstanceHandle InstantiateMaterialInstance( MaterialHandle a_Master, const string& a_Name = nullptr )		{ return MaterialLoader::GetInstance()->InstantiateMaterialInstance( a_Master, a_Name ); }
	static MaterialInstanceHandle InstantiateMaterialInstance( const string& a_MasterName, const string& a_Name = nullptr ) { return MaterialLoader::GetInstance()->InstantiateMaterialInstance( a_MasterName, a_Name ); }

	// - Textures -
	static TextureHandle LoadTexture( const string& a_Path, GLenum a_Target, GLenum a_InternalFormat, bool a_srgb = false ) { return TextureLoader::GetInstance()->LoadTexture( a_Path, a_Target, a_InternalFormat, a_srgb ); }
	static TextureHandle GetTexture( const std::string& a_Name ) { return TextureLoader::GetInstance()->GetTexture( a_Name ); }

	//static void LoadScene( const std::string& a_Path, std::vector< MeshHandle >& o_Meshes, std::vector< MaterialHandle >& o_Materials, std::vector< TextureHandle >& o_Textures );
}

