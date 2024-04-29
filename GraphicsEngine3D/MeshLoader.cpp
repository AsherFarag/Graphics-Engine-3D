#include "MeshLoader.h"
#include <assimp/scene.h>

MeshLoader::MeshLoader()
{
    // Load Primitive Shapes
    LoadMesh( "Content/Mesh/Primitives/Box.obj" );
    LoadMesh( "Content/Mesh/Primitives/Cone.obj" );
    LoadMesh( "Content/Mesh/Primitives/Cylinder.obj" );
    LoadMesh( "Content/Mesh/Primitives/Pyramid.obj" );
    LoadMesh( "Content/Mesh/Primitives/Sphere.obj" );
}

MeshLoader* MeshLoader::GetInstance()
{
    static MeshLoader Instance;
    return &Instance;
}

MeshHandle MeshLoader::LoadMesh( const string& a_Path, const string& a_Name, const aiScene* a_Scene )
{
    // Check if this mesh has already been loaded
    auto& foundMesh = m_LoadedMeshes.find( a_Name );
    if ( foundMesh != m_LoadedMeshes.end() )
        return foundMesh->second;

    MeshHandle mesh = std::make_shared< RMesh >();

    // === Load Mesh ===

    mesh->ProcessNode( a_Scene->mRootNode, a_Scene );

    // =================


    mesh->ConstuctResourceInfo( a_Path, a_Name );

    m_LoadedMeshes.emplace( mesh->GetResourceName(), mesh );

    return mesh;
}

MeshHandle MeshLoader::LoadMesh( const string& a_Path )
{
    Assimp::Importer importer;
    importer.SetPropertyBool( AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false );
    importer.SetPropertyFloat( AI_CONFIG_GLOBAL_SCALE_FACTOR_KEY, 1.f );

    unsigned int propertyFlags = aiProcess_GlobalScale
        | aiProcess_OptimizeMeshes
        | aiProcess_RemoveRedundantMaterials
        | aiProcess_PopulateArmatureData
        | aiProcess_Triangulate
        | aiPostProcessSteps::aiProcess_LimitBoneWeights
        //| aiPostProcessSteps::aiProcess_GenSmoothNormals
        //| aiProcess_ForceGenNormals 
        | aiPostProcessSteps::aiProcess_CalcTangentSpace
        | 0;

    const aiScene* Scene = importer.ReadFile( a_Path, propertyFlags);

    // Get the name of the File without the file extension
    string fileName = a_Path.substr( a_Path.find_last_of( '/' ) + 1 );
    fileName = fileName.substr( 0, fileName.find_last_of( '.' ) );


    return LoadMesh(a_Path, fileName, Scene);
}

MeshHandle MeshLoader::GetMesh( const string& a_Name )
{
    if ( auto it = m_LoadedMeshes.find( a_Name ); it != m_LoadedMeshes.end() )
    {
        return it->second;
    }
    return nullptr;
}

template < typename Func >
void ForEachBoneDescendent( const aiNode* a_Node, Func&& Function )
{
    for ( int i = 0; i < a_Node->mNumChildren; ++i )
    {
        Function( a_Node );
        ForEachBoneDescendent( a_Node->mChildren[ i ], Function );
    }
}


SkeletonHandle MeshLoader::LoadSkeleton( const string& a_Path, const string& a_Name, const aiNode* a_BoneRootNode, const aiScene* a_Scene )
{
    SkeletonHandle skeleton = std::make_shared<RSkeleton>();
    
    // Add root bone to the skeleton's bones
    skeleton->m_Bones.emplace_back().Name = a_BoneRootNode->mName.C_Str();

    // Recursive loop for adding each child bone 
    ForEachBoneDescendent( a_BoneRootNode,
        [&skeleton,a_Scene]( const aiNode* a_Node )
        {
            
            auto it = std::find_if( skeleton->m_Bones.begin(), skeleton->m_Bones.end(),
                [a_Node]( const Bone& a )
                {
                    return a.Name == a_Node->mParent->mName.C_Str();
                } );

            if ( it == skeleton->m_Bones.end() )
            {
                LOG( Error, ( "Child Bone doesn't have parent " + string( a_Node->mName.C_Str() ) ).c_str() );
                return;
            }

            int parentIndex = it - skeleton->m_Bones.begin();

            auto& bone = skeleton->m_Bones.emplace_back();
            bone.Name = a_Node->mName.C_Str();
            bone.Parent = parentIndex;

            // Assimp loads matricies as Row Major so we must convert it to Column major
            bone.BindTransform = Math::AssimpMatToGLM( a_Node->mTransformation );
            //bone.OffsetMatrix = Math::AssimpMatToGLM(  a_Scene->mSkeletons[ 0 ]->mBones[ parentIndex + 1 ]->mOffsetMatrix );
        }
    );
    
    skeleton->GenerateBoneData();

    skeleton->ConstuctResourceInfo( a_Path, a_Name );

    return skeleton;
}
