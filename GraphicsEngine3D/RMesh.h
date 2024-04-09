#pragma once
#include "RResource.h"

// --- Assimp ---
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// --- OpenGL ---
#include "gl_core_4_4.h"

// --- STD ---
#include <vector>

// --- Engine ---
#include "RMaterial.h"

#define MESH_FILE_PATH "Mesh/"

struct Vertex
{
	vec4 Position;	// Added to attribute location 0
	vec4 Normal;		// Added to attribute location 1
	vec2 TexCoord;	// Added to attribute location 2

	// Normal-Mapping Data
	vec4 Tangent;		// Added to attribute location 3
};

class MeshChunk
{
public:
	MeshChunk(std::vector<Vertex> a_Vertices, std::vector<unsigned int> a_Indices, std::vector<Texture> a_Textures)
	{
		Vertices = a_Vertices;
		Indices	 = a_Indices;
		Textures = a_Textures;

		Initialise();
	}

	~MeshChunk()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &IBO);
	}

	void Draw()
	{
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	std::vector<Vertex>       Vertices;
	std::vector<unsigned int> Indices;
	std::vector<Texture>      Textures;

	unsigned int MaterialID;

private:
	unsigned int VAO = 0; // The Vertex Array Object
	unsigned int VBO = 0; // The Vertex Buffer Object
	unsigned int IBO = 0; // The Index Buffer Object

	void Initialise()
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &IBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), &Vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int),
			&Indices[0], GL_STATIC_DRAW);

		// vertex positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		// vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::Normal));
		// vertex texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::TexCoord));

		glBindVertexArray(0);
	}
};

class RMesh
	: public RResource
{
public:
	RMesh(char* Path);

	void Draw();

private:
	std::vector<MeshChunk> m_MeshChunks;
	RMaterial* m_Materials;

	void LoadModel(string Path, unsigned int ProcessSteps = aiProcess_Triangulate | aiProcess_FlipUVs);
	// Assimp
	void ProcessNode(aiNode* a_Node, const aiScene* a_Scene);
	MeshChunk ProcessMeshChunk(aiMesh* a_Mesh, const aiScene* a_Scene);





public:

	enum EPrimativeShape
	{
		EPS_Quad,
		EPS_Box,
		EPS_Cylinder,
		EPS_Pyramid,
		EPS_Sphere,
		EPS_Cone,
	};

	void Initialise(unsigned int a_VertexCount, const Vertex* a_Vertices, unsigned int a_IndexCount = 0, unsigned int* a_Indices = nullptr);

	void Initialise(const char* a_FileName);

	void Initialise(EPrimativeShape a_PrimativeMesh);

	void InitialiseQuad();

	void InitialiseFullScreenQuad();

	// Will fail if it cannot find the file OR a mesh has already been loaded in this instance
	bool Load(const char* a_FileName, bool a_LoadTextures = true, bool a_FlipTextures = false);

	RMaterial* GetMaterial() { return m_Materials; }
	void SetMaterial(RMaterial* a_Material) { m_Materials = a_Material; }

	// Material access
	//size_t GetMaterialCount() const { return m_Materials.size(); }
	//RMaterial& GetMaterial(size_t index = 0) { return m_Materials[index]; }
};

