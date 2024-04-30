#include "ResourceManager.h"

// --- AssImp ---
#include "assimp/postprocess.h"
#include "assimp/scene.h"

//bool Resource::LoadFBX( const std::string& a_Path, std::vector<MeshHandle>& o_Meshes, std::vector<AnimationHandle>& o_Animations, std::vector<MaterialHandle>& o_Materials, std::vector<TextureHandle>& o_Textures, int a_ProcessSteps )
//{
//
//	Assimp::Importer Import;
//	const aiScene* Scene = Import.ReadFile( a_Path, a_ProcessSteps );
//
//	if ( !Scene || Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !Scene->mRootNode )
//	{
//		LOG( Error, Import.GetErrorString() );
//		return false;
//	}
//
//
//
//
//	return true;
//}

bool Resource::LoadFBX( const std::string& a_Path, ResourcePackage& o_Package )
{
	Assimp::Importer importer;
	importer.SetPropertyBool( AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false );
	importer.SetPropertyFloat( AI_CONFIG_GLOBAL_SCALE_FACTOR_KEY, 1.f );

	unsigned int propertyFlags =
		  aiProcess_GlobalScale
		| aiProcess_OptimizeMeshes
		| aiProcess_OptimizeGraph
		| aiProcess_RemoveRedundantMaterials
		| aiProcess_PopulateArmatureData
		| aiProcess_Triangulate
		| aiProcess_LimitBoneWeights
		| aiProcess_SplitByBoneCount
		| aiProcess_CalcTangentSpace;

	const aiScene* scene = importer.ReadFile( a_Path, propertyFlags );
	if ( !scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode )
	{
		LOG( Error, importer.GetErrorString() );
		return false;
	}

	auto animLoader = AnimationLoader::GetInstance();
	for ( int i = 0; i < scene->mNumAnimations; ++i )
	{
		o_Package.Animations.push_back( animLoader->LoadAnimation( scene->mAnimations[ i ] ) );
	}
}
