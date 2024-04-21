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
#include "RMaterialInstance.h"

#define MESH_FILE_PATH "Mesh/"

constexpr auto MAX_BONE_INFLUENCE = 4;

struct Vertex
{
	vec4 Position;	// Added to attribute location 0
	vec4 Normal;	// Added to attribute location 1
	vec2 TexCoord;	// Added to attribute location 2

	// Normal-Mapping Data
	vec3 Tangent;	// Added to attribute location 3
	vec3 Bitangent;	// Added to attribute location 4

	//bone indexes which will influence this vertex
	int m_BoneIDs[ MAX_BONE_INFLUENCE ];
	//weights from each bone
	float m_Weights[ MAX_BONE_INFLUENCE ];
};

class MeshChunk
{
public:
	MeshChunk() = default;
	MeshChunk(const MeshChunk& ) = default;
	MeshChunk( MeshChunk&& a_MeshChunk );

	MeshChunk& operator=( const MeshChunk& ) = default;
	MeshChunk& operator=( MeshChunk&& ) = default;

	MeshChunk( std::vector<Vertex> a_Vertices, std::vector<unsigned int> a_Indices );

	~MeshChunk();

	void Draw();

	std::vector<Vertex>       Vertices;
	std::vector<unsigned int> Indices;

	unsigned int MaterialID;

private:
	unsigned int VAO = 0; // The Vertex Array Object
	unsigned int VBO = 0; // The Vertex Buffer Object
	unsigned int IBO = 0; // The Index Buffer Object

	void Initialise();
};

class RMesh
	: public RResource
{
	friend class MeshLoader;

private:

	std::vector<MeshChunk> m_MeshChunks;
	MaterialInstanceHandle m_Material;

	bool Load( string Path, bool a_GenerateMaterials, unsigned int ProcessSteps = aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_CalcTangentSpace);
	// Assimp
	void ProcessNode( aiNode* a_Node, const aiScene* a_Scene );
	MeshChunk ProcessMeshChunk( aiMesh* a_Mesh, const aiScene* a_Scene, int a_Index );

public:
	void Draw();

#pragma region Legacy

	// Legacy
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

	void Initialise( unsigned int a_VertexCount, const Vertex* a_Vertices, unsigned int a_IndexCount = 0, unsigned int* a_Indices = nullptr ) {}

	void InitialiseQuad() {}

	void InitialiseFullScreenQuad() {}

	// Will fail if it cannot find the file OR a mesh has already been loaded in this instance
	//bool Load( const char* a_FileName, bool a_LoadTextures = true, bool a_FlipTextures = false );

	MaterialInstanceHandle GetMaterial() { return m_Material; }
	void SetMaterial( MaterialInstanceHandle a_Material ) { m_Material = a_Material; }

	// Material access
	//size_t GetMaterialCount() const { return m_Materials.size(); }
	//RMaterial& GetMaterial(size_t index = 0) { return m_Materials[index]; }
#pragma endregion
};

using MeshHandle = std::shared_ptr< RMesh >;