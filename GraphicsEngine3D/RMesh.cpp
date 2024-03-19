#include "RMesh.h"
// --- GLM ---
#include "gl_core_4_4.h"
#include <glm/geometric.hpp>

// --- ASSIMP ---
#include <assimp/scene.h>
#include <assimp/cimport.h>

RMesh::~RMesh()
{
	glDeleteVertexArrays(1, &m_MeshChunk.m_VAO);
	glDeleteBuffers(1, &m_MeshChunk.m_VBO);
	glDeleteBuffers(1, &m_MeshChunk.m_IBO);
}

void RMesh::Initialise(unsigned int a_VertexCount, const Vertex* a_Vertices, unsigned int a_IndexCount, unsigned int* a_Indices)
{
	assert(m_MeshChunk.m_VAO == 0);

	// generate buffers
	glGenBuffers(1, &m_MeshChunk.m_VBO);
	glGenVertexArrays(1, &m_MeshChunk.m_VAO);

	// bind vertex array aka a mesh wrapper
	glBindVertexArray(m_MeshChunk.m_VAO);

	// bind vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_MeshChunk.m_VBO);

	// fill vertex buffer
	glBufferData(GL_ARRAY_BUFFER, a_VertexCount * sizeof(Vertex), a_Vertices, GL_STATIC_DRAW);

	// enable first element as position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// bind indices if there are any
	if (a_IndexCount != 0) {
		glGenBuffers(1, &m_MeshChunk.m_IBO);

		// bind vertex buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_MeshChunk.m_IBO);

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
		Vertices[i].m_Position = glm::vec4(Mesh->mVertices[i].x,
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
	assert(m_MeshChunk.m_VAO == 0);

	// Generate the buffer
	glGenBuffers(1, &m_MeshChunk.m_VBO);
	glGenVertexArrays(1, &m_MeshChunk.m_VAO);

	// Bind the vertex array, this will be our mesh buffer
	glBindVertexArray(m_MeshChunk.m_VAO);

	// Bind the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_MeshChunk.m_VBO);

	// Define the 6 verticies for 2 triangles
	Vertex Vertices[6];
	Vertices[0].m_Position = { -0.5f, 0.f,  0.5f, 1 };
	Vertices[1].m_Position = {  0.5f, 0.f,  0.5f, 1 };
	Vertices[2].m_Position = { -0.5f, 0.f, -0.5f, 1 };

	Vertices[3].m_Position = { -0.5f, 0.f, -0.5f, 1 };
	Vertices[4].m_Position = {  0.5f, 0.f,  0.5f, 1 };
	Vertices[5].m_Position = {  0.5f, 0.f, -0.5f, 1 };

	Vertices[0].m_TexCoord = { 0,1 }; // Bottom Left
	Vertices[1].m_TexCoord = { 1,1 }; // Bottom Right
	Vertices[2].m_TexCoord = { 0,0 }; // Top Left
	Vertices[3].m_TexCoord = { 0,0 }; // Top Right
	Vertices[4].m_TexCoord = { 1,1 };
	Vertices[5].m_TexCoord = { 1,0 };

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
		Vertices[i].m_Position = glm::vec4(Mesh->mVertices[i].x,
			Mesh->mVertices[i].y, Mesh->mVertices[i].z, 1);
		// TODO, normals and UVs
	}
	Initialise(NumV, Vertices, Indices.size(), Indices.data());
	delete[] Vertices;

	return true;
}

void RMesh::Draw()
{
	glBindVertexArray(m_MeshChunk.m_VAO);
	if (m_MeshChunk.m_IBO != 0)
	{
		glDrawElements(GL_TRIANGLES, 3 * m_MeshChunk.m_TriCount, GL_UNSIGNED_INT, 0);
	}
	{
		glDrawArrays(GL_TRIANGLES, 0, 3 * m_MeshChunk.m_TriCount);
	}
}
