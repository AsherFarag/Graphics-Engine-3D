#include "RMesh.h"
// --- GLM ---
#include "gl_core_4_4.h"
#include <glm/geometric.hpp>

// --- ASSIMP ---
#include <assimp/scene.h>
#include <assimp/cimport.h>

RMesh::RMesh(char* Path)
{
	LoadModel(Path);
}

void RMesh::Draw()
{
	for (unsigned int i = 0; i < m_MeshChunks.size(); i++)
		m_MeshChunks[i].Draw();
}

void RMesh::LoadModel(string Path, unsigned int ProcessSteps)
{
	Assimp::Importer Import;
	const aiScene* Scene = Import.ReadFile(Path, ProcessSteps);

	if (!Scene || Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !Scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << Import.GetErrorString() << std::endl;
		LOG(Error, true, Import.GetErrorString());
		return;
	}

	ProcessNode(Scene->mRootNode, Scene);
}

void RMesh::ProcessNode(aiNode* a_Node, const aiScene* a_Scene)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < a_Node->mNumMeshes; i++)
	{
		aiMesh* mesh = a_Scene->mMeshes[a_Node->mMeshes[i]];
		m_MeshChunks.push_back(ProcessMeshChunk(mesh, a_Scene));
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < a_Node->mNumChildren; i++)
	{
		ProcessNode(a_Node->mChildren[i], a_Scene);
	}
}

MeshChunk RMesh::ProcessMeshChunk(aiMesh* a_Mesh, const aiScene* a_Scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	// Process Verticies
	for (unsigned int i = 0; i < a_Mesh->mNumVertices; ++i)
	{
		Vertex vertex;
		vertex.Position = vec4(a_Mesh->mVertices[i].x,		 a_Mesh->mVertices[i].y,	   a_Mesh->mVertices[i].z, 1);
		vertex.Normal	= vec4(a_Mesh->mNormals[i].x,		 a_Mesh->mNormals[i].y,		   a_Mesh->mNormals[i].z,  1);
		vertex.Tangent  = vec4(a_Mesh->mTangents[i].x,		 a_Mesh->mTangents[i].y,	   a_Mesh->mTangents[i].z, 1);
		vertex.TexCoord = vec2(a_Mesh->mTextureCoords[i]->x, a_Mesh->mTextureCoords[i]->y);

		vertices.push_back(vertex);
	}

	// Process Indicies
	for (int i = 0; i < a_Mesh->mNumFaces; i++)
	{
		indices.push_back(a_Mesh->mFaces[i].mIndices[0]);
		indices.push_back(a_Mesh->mFaces[i].mIndices[2]);
		indices.push_back(a_Mesh->mFaces[i].mIndices[1]);

		// generate a second triangle for quads
		if (a_Mesh->mFaces[i].mNumIndices == 4)
		{
			indices.push_back(a_Mesh->mFaces[i].mIndices[0]);
			indices.push_back(a_Mesh->mFaces[i].mIndices[3]);
			indices.push_back(a_Mesh->mFaces[i].mIndices[2]);
		}
	}

}

void RMesh::Initialise(unsigned int a_VertexCount, const Vertex* a_Vertices, unsigned int a_IndexCount, unsigned int* a_Indices)
{
	assert(m_MeshChunk.VAO == 0);

	// generate buffers
	glGenBuffers(1, &m_MeshChunk.VBO);
	glGenVertexArrays(1, &m_MeshChunk.VAO);

	// bind vertex array aka a mesh wrapper
	glBindVertexArray(m_MeshChunk.VAO);

	// bind vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_MeshChunk.VBO);

	// fill vertex buffer
	glBufferData(GL_ARRAY_BUFFER, a_VertexCount * sizeof(Vertex), a_Vertices, GL_STATIC_DRAW);

	// enable first element as position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// bind indices if there are any
	if (a_IndexCount != 0) {
		glGenBuffers(1, &m_MeshChunk.IBO);

		// bind vertex buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_MeshChunk.IBO);

		// fill vertex buffer
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, a_IndexCount * sizeof(unsigned int), a_Indices, GL_STATIC_DRAW);

		m_MeshChunk.m_TriCount = a_IndexCount / 3;
	}
	else {
		m_MeshChunk.m_TriCount = a_VertexCount / 3;
	}

	// unbind buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void RMesh::Initialise(const char* a_FileName)
{
	std::string StringFilePath = std::string(RESOURCE_PATH) + MESH_FILE_PATH + a_FileName;
	const char* FilePath = StringFilePath.c_str();
	// Read vertices from the model
	const aiScene* Scene = aiImportFile(FilePath, 0);

	aiMesh* Mesh = Scene->mMeshes[0];

	// Excract indicies from the Mesh
	int NumFaces = Mesh->mNumFaces;
	std::vector<unsigned int> Indices;
	for (int i = 0; i < NumFaces; i++)
	{
		Indices.push_back(Mesh->mFaces[i].mIndices[0]);
		Indices.push_back(Mesh->mFaces[i].mIndices[2]);
		Indices.push_back(Mesh->mFaces[i].mIndices[1]);
		// generate a second triangle for quads
		if (Mesh->mFaces[i].mNumIndices == 4)
		{
			Indices.push_back(Mesh->mFaces[i].mIndices[0]);
			Indices.push_back(Mesh->mFaces[i].mIndices[3]);
			Indices.push_back(Mesh->mFaces[i].mIndices[2]);
		}
	}

	// Extract vertex data
	int NumV = Mesh->mNumVertices;
	Vertex* Vertices = new Vertex[NumV];
	for (int i = 0; i < NumV; i++)
	{
		Vertices[i].Position = glm::vec4(Mesh->mVertices[i].x,
			Mesh->mVertices[i].y, Mesh->mVertices[i].z, 1);
		// TODO, normals and UVs
	}
	Initialise(NumV, Vertices, Indices.size(), Indices.data());
	delete[] Vertices;

}

void RMesh::Initialise(EPrimativeShape a_PrimativeMesh)
{
	std::string PrimativeFileName = "Primitives/";
	// Get num of verticies
	switch (a_PrimativeMesh)
	{
	case RMesh::EPS_Quad:
		InitialiseQuad();
		break;
	case RMesh::EPS_Box:
		PrimativeFileName.append("Box");
		break;
	case RMesh::EPS_Cylinder:
		PrimativeFileName.append("Cylinder");
		break;
	case RMesh::EPS_Pyramid:
		PrimativeFileName.append("Pyramid");
		break;
	case RMesh::EPS_Sphere:
		PrimativeFileName.append("Sphere");
		break;
	case RMesh::EPS_Cone:
		PrimativeFileName.append("Cone");
		break;
	default:
		break;
	}

	PrimativeFileName.append(".obj");
	Initialise(PrimativeFileName.c_str());
}

void RMesh::InitialiseQuad()
{
	// Check that it has not already been initialised
	assert(m_MeshChunk.VAO == 0);

	// Generate the buffer
	glGenBuffers(1, &m_MeshChunk.VBO);
	glGenVertexArrays(1, &m_MeshChunk.VAO);

	// Bind the vertex array, this will be our mesh buffer
	glBindVertexArray(m_MeshChunk.VAO);

	// Bind the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_MeshChunk.VBO);

	// Define the 6 verticies for 2 triangles
	Vertex Vertices[6];
	Vertices[0].Position = { -0.5f, 0.f,  0.5f, 1 };
	Vertices[1].Position = {  0.5f, 0.f,  0.5f, 1 };
	Vertices[2].Position = { -0.5f, 0.f, -0.5f, 1 };

	Vertices[3].Position = { -0.5f, 0.f, -0.5f, 1 };
	Vertices[4].Position = {  0.5f, 0.f,  0.5f, 1 };
	Vertices[5].Position = {  0.5f, 0.f, -0.5f, 1 };

	Vertices[0].TexCoord = { 0,1 }; // Bottom Left
	Vertices[1].TexCoord = { 1,1 }; // Bottom Right
	Vertices[2].TexCoord = { 0,0 }; // Top Left
	Vertices[3].TexCoord = { 0,0 }; // Top Right
	Vertices[4].TexCoord = { 1,1 };
	Vertices[5].TexCoord = { 1,0 };

	// Fill Vertex Buffer
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vertex), Vertices, GL_STATIC_DRAW);

	// Set the first element to be a position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// Unbind buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Set the Tri Count
	m_MeshChunk.m_TriCount = 2;
}

void RMesh::InitialiseFullScreenQuad()
{
	assert(m_MeshChunk.VAO == 0);

	// generate buffers
	glGenBuffers(1, &m_MeshChunk.VBO);
	glGenVertexArrays(1, &m_MeshChunk.VAO);

	// bind vertex array aka a mesh wrapper
	glBindVertexArray(m_MeshChunk.VAO);
	// bind vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_MeshChunk.VBO);

	// define vertices
	float vertices[] =
	{
	-1,1, // left top
	-1,-1, // left bottom
	1,1, // right top
	-1,-1, // left bottom
	1,-1, // right bottom
	1, 1 // right top
	};

	// fill vertex buffer
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertices,
		GL_STATIC_DRAW);
	// enable first element as position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8, 0);
	// unbind buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// quad has 2 triangles
	m_MeshChunk.m_TriCount = 2;
}

bool RMesh::Load(const char* a_FileName, bool a_LoadTextures, bool a_FlipTextures)
{
	//std::string StringFilePath = std::string(RESOURCE_PATH) + MESH_FILE_PATH + a_FileName;
	//a_FileName = StringFilePath.c_str();
	//if (m_meshChunks.empty() == false) {
	//	printf("Mesh already initialised, can't re-initialise!\n");
	//	return false;
	//}
	//return false;

	std::string StringFilePath = std::string(RESOURCE_PATH) + MESH_FILE_PATH + a_FileName;
	const char* FilePath = StringFilePath.c_str();
	// Read vertices from the model
	const aiScene* Scene = aiImportFile(FilePath, 0);

	aiMesh* Mesh = Scene->mMeshes[0];

	// Excract indicies from the Mesh
	int NumFaces = Mesh->mNumFaces;
	std::vector<unsigned int> Indices;
	for (int i = 0; i < NumFaces; i++)
	{
		Indices.push_back(Mesh->mFaces[i].mIndices[0]);
		Indices.push_back(Mesh->mFaces[i].mIndices[2]);
		Indices.push_back(Mesh->mFaces[i].mIndices[1]);
		// generate a second triangle for quads
		if (Mesh->mFaces[i].mNumIndices == 4)
		{
			Indices.push_back(Mesh->mFaces[i].mIndices[0]);
			Indices.push_back(Mesh->mFaces[i].mIndices[3]);
			Indices.push_back(Mesh->mFaces[i].mIndices[2]);
		}
	}

	// Extract vertex data
	int NumV = Mesh->mNumVertices;
	Vertex* Vertices = new Vertex[NumV];
	for (int i = 0; i < NumV; i++)
	{
		Vertices[i].Position = glm::vec4(Mesh->mVertices[i].x,
			Mesh->mVertices[i].y, Mesh->mVertices[i].z, 1);
		// TODO, normals and UVs
	}
	Initialise(NumV, Vertices, Indices.size(), Indices.data());
	delete[] Vertices;

	return true;
}

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
