#pragma once

// --- STD ---
#include <string>
#include <map>
using std::string;
using std::map;

// --- AssImp ---
#include "assimp/Importer.hpp"

// --- Engine ---
#include "ShaderLoader.h"
#include "Animation.h"

namespace Resource
{
	struct ResourcePackage
	{
		MeshHandle Mesh = nullptr;
		std::vector<SkeletonHandle>     Skeletons;
		std::vector<SkeletalAnimHandle> Animations;
		std::vector<MaterialHandle>     Materials;
		std::vector<TextureHandle>      Textures;

		const bool HasMesh()	   const { return Mesh != nullptr; }
		const bool HasSkeletons()  const { return Skeletons.size() > 0; }
		const bool HasAnimations() const { return Animations.size() > 0; }
		const bool HasMaterials()  const { return Materials.size() > 0; }
		const bool HasTextures()   const { return Textures.size() > 0; }
	};

	// - Meshes -
	//static MeshHandle LoadMesh( const std::string& a_Path ) { return MeshLoader::GetInstance()->LoadMesh( a_Path ); }
	static MeshHandle GetMesh( const std::string& a_Name ) { return MeshLoader::GetInstance()->GetMesh( a_Name ); }

	// - Shaders -
	static ShaderHandle LoadShader( const std::string& a_Path ) { return ShaderLoader::GetInstance()->LoadShader( a_Path ); }
	static ShaderHandle GetShader( const std::string& a_Name ) { return ShaderLoader::GetInstance()->GetShader( a_Name ); }

	// - Materials -
	static MaterialHandle GetMaterial( const string& a_Name ) { return MaterialLoader::GetInstance()->GetMaterial( a_Name ); }

	// - Textures -
	static TextureHandle LoadTexture( const string& a_Path, GLenum a_Target, GLenum a_InternalFormat, bool a_srgb = false ) { return TextureLoader::GetInstance()->LoadTexture( a_Path, a_Target, a_InternalFormat, a_srgb ); }
	static TextureHandle GetTexture( const std::string& a_Name ) { return TextureLoader::GetInstance()->GetTexture( a_Name ); }

	static bool LoadFBX( const std::string& a_Path, ResourcePackage& o_Package );
}

