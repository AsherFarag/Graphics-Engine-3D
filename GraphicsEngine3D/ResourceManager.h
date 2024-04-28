#pragma once

// --- STD ---
#include <string>
#include <map>
using std::string;
using std::map;

// --- AssImp ---
#include "assimp/Importer.hpp"

// --- Engine ---
#include "MeshLoader.h"
#include "ShaderLoader.h"
#include "MaterialLoader.h"
#include "TextureLoader.h"
#include "AnimationLoader.h"

namespace Resource
{
	// - Meshes -
	static MeshHandle LoadMesh( const std::string& a_Path ) { return MeshLoader::GetInstance()->LoadMesh( a_Path ); }
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

	static bool LoadFBX( const std::string& a_Path, std::vector< MeshHandle >& o_Meshes, std::vector< AnimationHandle >& o_Animations, std::vector< MaterialHandle >& o_Materials, std::vector< TextureHandle >& o_Textures, int a_ProcessSteps = aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_CalcTangentSpace );
}

