
#include "gl_core_4_4.h"

// --- ASSIMP ---
#include <assimp/scene.h>
#include <assimp/cimport.h>

//void RMesh::Draw()
//{
//	for ( unsigned int i = 0; i < m_MeshChunks.size(); i++ )
//		m_MeshChunks[ i ].Draw();
//}

//void RMesh::ProcessNode( aiNode* a_Node, const aiScene* a_Scene )
//{
//	m_MeshChunks.resize( a_Scene->mNumMeshes );
//
//	// process all the node's meshes (if any)
//	for ( unsigned int i = 0; i < a_Node->mNumMeshes; i++ )
//	{
//		aiMesh* mesh = a_Scene->mMeshes[ a_Node->mMeshes[ i ] ];
//		ProcessMeshChunk( m_MeshChunks[ i ], mesh, a_Scene );
//	}
//	// then do the same for each of its children
//	for ( unsigned int i = 0; i < a_Node->mNumChildren; i++ )
//	{
//		ProcessNode( a_Node->mChildren[ i ], a_Scene );
//	}
//}