#include "RMesh.h"
// --- GLM ---
#include "gl_core_4_4.h"
#include <glm/geometric.hpp>

// --- ASSIMP ---
#include <assimp/scene.h>
#include <assimp/cimport.h>

void RMesh::Draw()
{
	for ( unsigned int i = 0; i < m_MeshChunks.size(); i++ )
		m_MeshChunks[ i ].Draw();
}

bool RMesh::Load( string Path, bool a_GenerateMaterials, int ProcessSteps )
{
	Assimp::Importer Import;
	const aiScene* Scene = Import.ReadFile( Path, ProcessSteps );

	if ( !Scene || Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !Scene->mRootNode )
	{
		LOG( Error, Import.GetErrorString() );
		return false;
	}

	ConstuctResourceInfo( Path );

	ProcessNode( Scene->mRootNode, Scene );
	return true;
}

void RMesh::Load( aiScene* a_Scene )
{
	
}

void RMesh::ProcessNode( aiNode* a_Node, const aiScene* a_Scene )
{
	// process all the node's meshes (if any)
	for ( unsigned int i = 0; i < a_Node->mNumMeshes; i++ )
	{
		aiMesh* mesh = a_Scene->mMeshes[ a_Node->mMeshes[ i ] ];
		m_MeshChunks.push_back( ProcessMeshChunk( mesh, a_Scene, i ) );
	}
	// then do the same for each of its children
	for ( unsigned int i = 0; i < a_Node->mNumChildren; i++ )
	{
		ProcessNode( a_Node->mChildren[ i ], a_Scene );
	}
}

MeshChunk RMesh::ProcessMeshChunk( aiMesh* a_Mesh, const aiScene* a_Scene, int a_Index )
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	// Process Verticies
	for ( unsigned int i = 0; i < a_Mesh->mNumVertices; ++i )
	{
		Vertex vertex;
		vertex.Position	= vec4( a_Mesh->mVertices[ i ].x, a_Mesh->mVertices[ i ].y, a_Mesh->mVertices[ i ].z, 1 );

		if ( a_Mesh->HasNormals() )
		{
			vertex.Normal = vec4( a_Mesh->mNormals[ i ].x, a_Mesh->mNormals[ i ].y, a_Mesh->mNormals[ i ].z, 1 );
		}

		if ( a_Mesh->HasTangentsAndBitangents() )
		{
			vertex.Tangent = vec3( a_Mesh->mTangents[ i ].x, a_Mesh->mTangents[ i ].y, a_Mesh->mTangents[ i ].z );
			vertex.Bitangent = vec3( a_Mesh->mBitangents[ i ].x, a_Mesh->mBitangents[ i ].y, a_Mesh->mBitangents[ i ].z );
		}

		if ( a_Mesh->HasTextureCoords( i ) )
		{
			vertex.TexCoord = vec2( a_Mesh->mTextureCoords[ i ]->x, a_Mesh->mTextureCoords[ i ]->y );
		}

		vertices.push_back( vertex );
	}

	// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for ( unsigned int i = 0; i < a_Mesh->mNumFaces; i++ )
	{
		aiFace face = a_Mesh->mFaces[ i ];
		// retrieve all indices of the face and store them in the indices vector
		for ( unsigned int j = 0; j < face.mNumIndices; j++ )
			indices.push_back( face.mIndices[ j ] );
	}

	//// Process Indicies
	//for ( int i = 0; i < a_Mesh->mNumFaces; i++ )
	//{
	//	indices.push_back( a_Mesh->mFaces[ i ].mIndices[ 0 ] );
	//	indices.push_back( a_Mesh->mFaces[ i ].mIndices[ 2 ] );
	//	indices.push_back( a_Mesh->mFaces[ i ].mIndices[ 1 ] );

	//	// generate a second triangle for quads
	//	if ( a_Mesh->mFaces[ i ].mNumIndices == 4 )
	//	{
	//		indices.push_back( a_Mesh->mFaces[ i ].mIndices[ 0 ] );
	//		indices.push_back( a_Mesh->mFaces[ i ].mIndices[ 3 ] );
	//		indices.push_back( a_Mesh->mFaces[ i ].mIndices[ 2 ] );
	//	}
	//}

	if ( a_Mesh->mMaterialIndex >= 0 )
	{
		//a_Scene.tex
	}

	// Process Materials
	TODO( "Process Materials when loading meshes" );
	MeshChunk Temp( vertices, indices );
	return std::move(Temp);
}

//void RMesh::Initialise(unsigned int a_VertexCount, const Vertex* a_Vertices, unsigned int a_IndexCount, unsigned int* a_Indices)
//{
//	assert(m_MeshChunk.VAO == 0);
//
//	// generate buffers
//	glGenBuffers(1, &m_MeshChunk.VBO);
//	glGenVertexArrays(1, &m_MeshChunk.VAO);
//
//	// bind vertex array aka a mesh wrapper
//	glBindVertexArray(m_MeshChunk.VAO);
//
//	// bind vertex buffer
//	glBindBuffer(GL_ARRAY_BUFFER, m_MeshChunk.VBO);
//
//	// fill vertex buffer
//	glBufferData(GL_ARRAY_BUFFER, a_VertexCount * sizeof(Vertex), a_Vertices, GL_STATIC_DRAW);
//
//	// enable first element as position
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
//
//	// bind indices if there are any
//	if (a_IndexCount != 0) {
//		glGenBuffers(1, &m_MeshChunk.IBO);
//
//		// bind vertex buffer
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_MeshChunk.IBO);
//
//		// fill vertex buffer
//		glBufferData(GL_ELEMENT_ARRAY_BUFFER, a_IndexCount * sizeof(unsigned int), a_Indices, GL_STATIC_DRAW);
//
//		m_MeshChunk.m_TriCount = a_IndexCount / 3;
//	}
//	else {
//		m_MeshChunk.m_TriCount = a_VertexCount / 3;
//	}
//
//	// unbind buffers
//	glBindVertexArray(0);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//}
//
//void RMesh::Initialise(const char* a_FileName)
//{
//	std::string StringFilePath = std::string(RESOURCE_PATH) + MESH_FILE_PATH + a_FileName;
//	const char* FilePath = StringFilePath.c_str();
//	// Read vertices from the model
//	const aiScene* Scene = aiImportFile(FilePath, 0);
//
//	aiMesh* Mesh = Scene->mMeshes[0];
//
//	// Excract indicies from the Mesh
//	int NumFaces = Mesh->mNumFaces;
//	std::vector<unsigned int> Indices;
//	for (int i = 0; i < NumFaces; i++)
//	{
//		Indices.push_back(Mesh->mFaces[i].mIndices[0]);
//		Indices.push_back(Mesh->mFaces[i].mIndices[2]);
//		Indices.push_back(Mesh->mFaces[i].mIndices[1]);
//		// generate a second triangle for quads
//		if (Mesh->mFaces[i].mNumIndices == 4)
//		{
//			Indices.push_back(Mesh->mFaces[i].mIndices[0]);
//			Indices.push_back(Mesh->mFaces[i].mIndices[3]);
//			Indices.push_back(Mesh->mFaces[i].mIndices[2]);
//		}
//	}
//
//	// Extract vertex data
//	int NumV = Mesh->mNumVertices;
//	Vertex* Vertices = new Vertex[NumV];
//	for (int i = 0; i < NumV; i++)
//	{
//		Vertices[i].Position = glm::vec4(Mesh->mVertices[i].x,
//			Mesh->mVertices[i].y, Mesh->mVertices[i].z, 1);
//		// TODO, normals and UVs
//	}
//	Initialise(NumV, Vertices, Indices.size(), Indices.data());
//	delete[] Vertices;
//
//}
//
//void RMesh::Initialise(EPrimativeShape a_PrimativeMesh)
//{
//	std::string PrimativeFileName = "Primitives/";
//	// Get num of verticies
//	switch (a_PrimativeMesh)
//	{
//	case RMesh::EPS_Quad:
//		InitialiseQuad();
//		break;
//	case RMesh::EPS_Box:
//		PrimativeFileName.append("Box");
//		break;
//	case RMesh::EPS_Cylinder:
//		PrimativeFileName.append("Cylinder");
//		break;
//	case RMesh::EPS_Pyramid:
//		PrimativeFileName.append("Pyramid");
//		break;
//	case RMesh::EPS_Sphere:
//		PrimativeFileName.append("Sphere");
//		break;
//	case RMesh::EPS_Cone:
//		PrimativeFileName.append("Cone");
//		break;
//	default:
//		break;
//	}
//
//	PrimativeFileName.append(".obj");
//	Initialise(PrimativeFileName.c_str());
//}
//
//void RMesh::InitialiseQuad()
//{
//	// Check that it has not already been initialised
//	assert(m_MeshChunk.VAO == 0);
//
//	// Generate the buffer
//	glGenBuffers(1, &m_MeshChunk.VBO);
//	glGenVertexArrays(1, &m_MeshChunk.VAO);
//
//	// Bind the vertex array, this will be our mesh buffer
//	glBindVertexArray(m_MeshChunk.VAO);
//
//	// Bind the vertex buffer
//	glBindBuffer(GL_ARRAY_BUFFER, m_MeshChunk.VBO);
//
//	// Define the 6 verticies for 2 triangles
//	Vertex Vertices[6];
//	Vertices[0].Position = { -0.5f, 0.f,  0.5f, 1 };
//	Vertices[1].Position = {  0.5f, 0.f,  0.5f, 1 };
//	Vertices[2].Position = { -0.5f, 0.f, -0.5f, 1 };
//
//	Vertices[3].Position = { -0.5f, 0.f, -0.5f, 1 };
//	Vertices[4].Position = {  0.5f, 0.f,  0.5f, 1 };
//	Vertices[5].Position = {  0.5f, 0.f, -0.5f, 1 };
//
//	Vertices[0].TexCoord = { 0,1 }; // Bottom Left
//	Vertices[1].TexCoord = { 1,1 }; // Bottom Right
//	Vertices[2].TexCoord = { 0,0 }; // Top Left
//	Vertices[3].TexCoord = { 0,0 }; // Top Right
//	Vertices[4].TexCoord = { 1,1 };
//	Vertices[5].TexCoord = { 1,0 };
//
//	// Fill Vertex Buffer
//	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vertex), Vertices, GL_STATIC_DRAW);
//
//	// Set the first element to be a position
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
//
//	// Unbind buffers
//	glBindVertexArray(0);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//	// Set the Tri Count
//	m_MeshChunk.m_TriCount = 2;
//}
//
//void RMesh::InitialiseFullScreenQuad()
//{
//	assert(m_MeshChunk.VAO == 0);
//
//	// generate buffers
//	glGenBuffers(1, &m_MeshChunk.VBO);
//	glGenVertexArrays(1, &m_MeshChunk.VAO);
//
//	// bind vertex array aka a mesh wrapper
//	glBindVertexArray(m_MeshChunk.VAO);
//	// bind vertex buffer
//	glBindBuffer(GL_ARRAY_BUFFER, m_MeshChunk.VBO);
//
//	// define vertices
//	float vertices[] =
//	{
//	-1,1, // left top
//	-1,-1, // left bottom
//	1,1, // right top
//	-1,-1, // left bottom
//	1,-1, // right bottom
//	1, 1 // right top
//	};
//
//	// fill vertex buffer
//	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertices,
//		GL_STATIC_DRAW);
//	// enable first element as position
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8, 0);
//	// unbind buffers
//	glBindVertexArray(0);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	// quad has 2 triangles
//	m_MeshChunk.m_TriCount = 2;
//}
//
//bool RMesh::Load(const char* a_FileName, bool a_LoadTextures, bool a_FlipTextures)
//{
//	//std::string StringFilePath = std::string(RESOURCE_PATH) + MESH_FILE_PATH + a_FileName;
//	//a_FileName = StringFilePath.c_str();
//	//if (m_meshChunks.empty() == false) {
//	//	printf("Mesh already initialised, can't re-initialise!\n");
//	//	return false;
//	//}
//	//return false;
//
//	std::string StringFilePath = std::string(RESOURCE_PATH) + MESH_FILE_PATH + a_FileName;
//	const char* FilePath = StringFilePath.c_str();
//	// Read vertices from the model
//	const aiScene* Scene = aiImportFile(FilePath, 0);
//
//	aiMesh* Mesh = Scene->mMeshes[0];
//
//	// Excract indicies from the Mesh
//	int NumFaces = Mesh->mNumFaces;
//	std::vector<unsigned int> Indices;
//	for (int i = 0; i < NumFaces; i++)
//	{
//		Indices.push_back(Mesh->mFaces[i].mIndices[0]);
//		Indices.push_back(Mesh->mFaces[i].mIndices[2]);
//		Indices.push_back(Mesh->mFaces[i].mIndices[1]);
//		// generate a second triangle for quads
//		if (Mesh->mFaces[i].mNumIndices == 4)
//		{
//			Indices.push_back(Mesh->mFaces[i].mIndices[0]);
//			Indices.push_back(Mesh->mFaces[i].mIndices[3]);
//			Indices.push_back(Mesh->mFaces[i].mIndices[2]);
//		}
//	}
//
//	// Extract vertex data
//	int NumV = Mesh->mNumVertices;
//	Vertex* Vertices = new Vertex[NumV];
//	for (int i = 0; i < NumV; i++)
//	{
//		Vertices[i].Position = glm::vec4(Mesh->mVertices[i].x,
//			Mesh->mVertices[i].y, Mesh->mVertices[i].z, 1);
//		// TODO, normals and UVs
//	}
//	Initialise(NumV, Vertices, Indices.size(), Indices.data());
//	delete[] Vertices;
//
//	return true;
//}

//void RMesh::Draw()
//{
//	glBindVertexArray(m_MeshChunk.VAO);
//	if (m_MeshChunk.IBO != 0)
//	{
//		glDrawElements(GL_TRIANGLES, 3 * m_MeshChunk.m_TriCount, GL_UNSIGNED_INT, 0);
//	}
//	{
//		glDrawArrays(GL_TRIANGLES, 0, 3 * m_MeshChunk.m_TriCount);
//	}
//}

MeshChunk::MeshChunk( MeshChunk&& a_MeshChunk )
	: Vertices( std::move( a_MeshChunk.Vertices ) )
	, Indices( std::move( a_MeshChunk.Indices ) )
	, MaterialID( a_MeshChunk.MaterialID )
	, VAO( a_MeshChunk.VAO )
	, VBO( a_MeshChunk.VBO )
	, IBO( a_MeshChunk.IBO )
{
	a_MeshChunk.MaterialID = 0;
	a_MeshChunk.VAO = 0;
	a_MeshChunk.VBO = 0;
	a_MeshChunk.IBO = 0;
}

MeshChunk::MeshChunk( std::vector<Vertex> a_Vertices, std::vector<unsigned int> a_Indices )
{
	Vertices = a_Vertices;
	Indices = a_Indices;

	Initialise();
}

MeshChunk::~MeshChunk()
{
	glDeleteVertexArrays( 1, &VAO );
	glDeleteBuffers( 1, &VBO );
	glDeleteBuffers( 1, &IBO );
}

void MeshChunk::Draw()
{
	glBindVertexArray( VAO );
	glDrawElements( GL_TRIANGLES, static_cast<unsigned int>( Indices.size() ), GL_UNSIGNED_INT, 0 );
	glBindVertexArray( 0 );
}

void MeshChunk::Initialise()
{
	glGenVertexArrays( 1, &VAO );
	glGenBuffers( 1, &VBO );
	glGenBuffers( 1, &IBO );

	glBindVertexArray( VAO );
	glBindBuffer( GL_ARRAY_BUFFER, VBO );

	glBufferData( GL_ARRAY_BUFFER, Vertices.size() * sizeof( Vertex ), &Vertices[ 0 ], GL_STATIC_DRAW );

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, IBO );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof( unsigned int ), &Indices[ 0 ], GL_STATIC_DRAW );

	// vertex positions
	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), (void*)0 );
	// vertex normals
	glEnableVertexAttribArray( 1 );
	glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), (void*)offsetof( Vertex, Vertex::Normal ) );
	// vertex texture coords
	glEnableVertexAttribArray( 2 );
	glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, sizeof( Vertex ), (void*)offsetof( Vertex, Vertex::TexCoord ) );
	// vertex tangent
	glEnableVertexAttribArray( 3 );
	glVertexAttribPointer( 3, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), (void*)offsetof( Vertex, Vertex::Tangent ) );
	// vertex bitangent
	glEnableVertexAttribArray( 4 );
	glVertexAttribPointer( 4, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), (void*)offsetof( Vertex, Vertex::Bitangent ) );
	// ids
	glEnableVertexAttribArray( 5 );
	glVertexAttribIPointer( 5, 4, GL_INT, sizeof( Vertex ), (void*)offsetof( Vertex, Vertex::m_BoneIDs ) );
	// weights
	glEnableVertexAttribArray( 6 );
	glVertexAttribPointer( 6, 4, GL_FLOAT, GL_FALSE, sizeof( Vertex ), (void*)offsetof( Vertex, Vertex::m_Weights ) );

	glBindVertexArray( 0 );
}
