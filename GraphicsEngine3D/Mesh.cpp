#include "Mesh.h"

#pragma region Vertex

Vertex::Vertex()
{
	for ( int i = 0; i < MAX_BONE_INFLUENCE; i++ )
	{
		BoneIDs[ i ] = -1;
		Weights[ i ] = 0.0f;
	}
}

Vertex::Vertex( const aiMesh* a_Mesh, const int a_Index )
	: Vertex()
{
	// Position
	Position = Math::AssimpVecToGLM( a_Mesh->mVertices[ a_Index ], 1.f );

	// Normals
	if ( a_Mesh->HasNormals() )
	{
		Normal = Math::AssimpVecToGLM( a_Mesh->mNormals[ a_Index ], 1.f );
	}

	// Tangents and BiTangents
	if ( a_Mesh->HasTangentsAndBitangents() )
	{
		Tangent = Math::AssimpVecToGLM( a_Mesh->mTangents[ a_Index ] );
		Bitangent = Math::AssimpVecToGLM( a_Mesh->mBitangents[ a_Index ] );
	}

	// Texture Coords
	// Loops through the 8 UV channels
	for ( int i = 0; i < 8; ++i )
	{
		if ( a_Mesh->mTextureCoords[ i ] )
			TexCoords[i] = Math::AssimpVecToGLM( a_Mesh->mTextureCoords[ i ][ a_Index ] );
	}
}

void Vertex::SetBoneData( int a_BoneID, float a_Weight )
{
	for ( int i = 0; i < MAX_BONE_INFLUENCE; ++i )
	{
		if ( BoneIDs[ i ] < 0 )
		{
			Weights[ i ] = a_Weight;
			BoneIDs[ i ] = a_BoneID;
			break;
		}
	}
}

#pragma endregion

#pragma region Mesh

RMesh::RMesh( std::vector<Vertex> a_Vertices, std::vector<unsigned int> a_Indices )
{
	m_Vertices = a_Vertices;
	m_Indices = a_Indices;

	Initialise();
}

RMesh::~RMesh()
{
	glDeleteVertexArrays( 1, &VAO );
	glDeleteBuffers( 1, &VBO );
	glDeleteBuffers( 1, &IBO );
}

void RMesh::Draw()
{
	glBindVertexArray( VAO );
	glDrawElements( GL_TRIANGLES, static_cast<unsigned int>( m_Indices.size() ), GL_UNSIGNED_INT, 0 );
	glBindVertexArray( 0 );
}

void RMesh::Initialise()
{
	glGenVertexArrays( 1, &VAO );
	glGenBuffers( 1, &VBO );
	glGenBuffers( 1, &IBO );

	glBindVertexArray( VAO );
	glBindBuffer( GL_ARRAY_BUFFER, VBO );

	glBufferData( GL_ARRAY_BUFFER, m_Vertices.size() * sizeof( Vertex ), &m_Vertices[ 0 ], GL_STATIC_DRAW );

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, IBO );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof( unsigned int ), &m_Indices[ 0 ], GL_STATIC_DRAW );

	// vertex positions
	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), (void*)offsetof( Vertex, Position ) );
	// vertex normals
	glEnableVertexAttribArray( 1 );
	glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), (void*)offsetof( Vertex, Normal ) );
	// vertex tangent
	glEnableVertexAttribArray( 2 );
	glVertexAttribPointer( 2, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), (void*)offsetof( Vertex, Tangent ) );
	// vertex bitangent
	glEnableVertexAttribArray( 3 );
	glVertexAttribPointer( 3, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), (void*)offsetof( Vertex, Bitangent ) );
	// vertex texture coords
	glEnableVertexAttribArray( 4 );
	glVertexAttribPointer( 4, 2 * NUM_UV_CHANNELS, GL_FLOAT, GL_FALSE, sizeof( Vertex ), (void*)offsetof( Vertex, TexCoords ) );
	// Bone Ids
	glEnableVertexAttribArray( 5 );
	glVertexAttribIPointer( 5, 4, GL_INT, sizeof( Vertex ), (void*)offsetof( Vertex, BoneIDs ) );
	// Bone Weights
	glEnableVertexAttribArray( 6 );
	glVertexAttribPointer( 6, 4, GL_FLOAT, GL_FALSE, sizeof( Vertex ), (void*)offsetof( Vertex, Weights ) );

	glBindVertexArray( 0 );
}

void RMesh::SetMaterial( MaterialHandle a_Material )
{
	m_Material = a_Material;
}

#pragma endregion

#pragma region Model

void RModel::Draw()
{
	for ( auto mesh : m_Meshes )
	{
		mesh->Draw();
	}
}

#pragma endregion

#pragma region Loader

#pragma region Helper Functions

void ExtractBoneWeightsForMesh( BoneInfoMap& a_BoneInfoMap, std::vector<Vertex>& a_Vertices, const aiMesh* a_aiMesh )
{
	// Iterate through each bone
	for ( int boneIndex = 0; boneIndex < a_aiMesh->mNumBones; ++boneIndex )
	{
		int boneID = -1;
		std::string boneName = a_aiMesh->mBones[ boneIndex ]->mName.C_Str();
		// If this bone is not in the bone info map,
		// Construct a new Bone Info and add it
		if ( a_BoneInfoMap.find( boneName ) == a_BoneInfoMap.end() )
		{
			boneID = a_BoneInfoMap.size();
			mat4 offset = Math::AssimpMatToGLM( a_aiMesh->mBones[ boneIndex ]->mOffsetMatrix );
			a_BoneInfoMap.emplace( boneName, BoneInfo( boneID, offset ) );
		}
		else
		{
			boneID = a_BoneInfoMap[ boneName ].ID;
		}

		assert( boneID != -1 );
		auto weights = a_aiMesh->mBones[ boneIndex ]->mWeights;
		auto numWeights = a_aiMesh->mBones[ boneIndex ]->mNumWeights;

		// Assign the bone ID and corresponding weight to the vertex
		for ( int weightIndex = 0; weightIndex < numWeights; ++weightIndex )
		{
			int vertexId = weights[ weightIndex ].mVertexId;
			float weight = weights[ weightIndex ].mWeight;
			assert( vertexId <= a_Vertices.size() );
			a_Vertices[ vertexId ].SetBoneData( boneID, weight );
		}
	}
}

#pragma endregion



MeshHandle MeshLoader::LoadMesh( const aiMesh* a_Mesh )
{
	MeshHandle newMesh = std::make_shared<RMesh>();

	// Process Verticies
	auto& verticies = newMesh->GetVerticies();
	verticies.reserve( a_Mesh->mNumVertices );
	for ( unsigned int i = 0; i < a_Mesh->mNumVertices; ++i )
	{
		verticies.emplace_back( a_Mesh, i );
	}
	 
	// Process Indicies per the Meshes Faces
	auto& indices = newMesh->GetIndices();
	for ( unsigned int i = 0; i < a_Mesh->mNumFaces; i++ )
	{
		aiFace face = a_Mesh->mFaces[ i ];
		for ( unsigned int j = 0; j < face.mNumIndices; j++ )
			indices.push_back( face.mIndices[ j ] );
	}

	if ( a_Mesh->mMaterialIndex >= 0 )
	{
		TODO( "Process Materials when loading meshes" );
	}

	newMesh->Initialise();

	return newMesh;
}

MeshHandle MeshLoader::GetMesh( const string& a_Name )
{
	if ( auto it = m_LoadedMeshes.find( a_Name ); it != m_LoadedMeshes.end() )
	{
		return it->second;
	}
	return nullptr;
}

ModelHandle MeshLoader::LoadModel( const aiScene* a_Scene )
{
	ModelHandle newModel = std::make_shared<RModel>();
}

ModelHandle MeshLoader::GetMesh( const string& a_Name )
{
	if ( auto it = m_LoadedModels.find( a_Name ); it != m_LoadedModels.end() )
	{
		return it->second;
	}
	return nullptr;
}

#pragma endregion

