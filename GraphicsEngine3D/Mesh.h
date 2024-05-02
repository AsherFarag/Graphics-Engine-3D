#pragma once
#include "RResource.h"
#include "Material.h"

// --- ASSIMP ---
#include <assimp/mesh.h>
#include <assimp/scene.h>

#pragma region Structs

constexpr int MAX_BONE_INFLUENCE = 4;
constexpr int NUM_UV_CHANNELS = 8;

struct Vertex
{
	Vertex();
	Vertex( const aiMesh* a_Mesh, const int a_Index );

	vec4 Position;		// Added to attribute location 0
	vec4 Normal;		// Added to attribute location 1
	vec3 Tangent;		// Added to attribute location 2
	vec3 Bitangent;		// Added to attribute location 3

	vec2 TexCoords[ NUM_UV_CHANNELS ];	 // Added to attribute location 4
	int BoneIDs[ MAX_BONE_INFLUENCE ]; 	 // Added to attribute location 5
	float Weights[ MAX_BONE_INFLUENCE ]; // Added to attribute location 6

	void SetBoneData( int a_BoneID, float a_Weight );
};

struct BoneInfo
{
	BoneInfo( unsigned int a_ID, mat4 a_Offset ) : ID(a_ID), Offset(a_Offset) {}

	/* The the index in the Final Bone Matrices of a Skeletal Animation Pose */
	unsigned int ID;
	/* The offset matrix transforms vertex from model space to bone space */
	mat4 Offset;
};

#pragma endregion



#pragma region Mesh

class RMesh : public RResource
{
public:
	RMesh() = default;
	RMesh( std::vector<Vertex> a_Vertices, std::vector<unsigned int> a_Indices );
	~RMesh();

	void Draw();
	/* Initialises the data of the mesh onto the GPU with OpenGL. */
	void Initialise();

	auto& GetVerticies() { return m_Vertices; }
	auto& GetIndices()	 { return m_Indices; }
	auto  GetMaterial()  { return m_Material; }
	void  SetMaterial( MaterialHandle a_Material );

private:
	unsigned int VAO = 0; // The Vertex Array Object
	unsigned int VBO = 0; // The Vertex Buffer Object
	unsigned int IBO = 0; // The Index Buffer Object

	std::vector<Vertex>       m_Vertices;
	std::vector<unsigned int> m_Indices;
	MaterialHandle m_Material;
};

using MeshHandle = std::shared_ptr<RMesh>;

#pragma endregion



#pragma region Loader

class MeshLoader
{
private:
	MeshLoader();
	~MeshLoader() = default;

	std::map<string, MeshHandle> m_LoadedMeshes;

public:
	MeshLoader( MeshLoader& Other ) = delete;
	void operator=( const MeshLoader& ) = delete;

	static MeshLoader* GetInstance();

	MeshHandle LoadMesh( const aiMesh* a_Mesh );
	MeshHandle GetMesh( const string& a_Name );
	const auto& GetLoadedMeshes() const { return m_LoadedMeshes; }
};

#pragma endregion


