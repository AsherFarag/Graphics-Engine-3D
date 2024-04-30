#pragma once
#include "RResource.h"
#include "Material.h"

#pragma region Structs

constexpr auto MAX_BONE_INFLUENCE = 4;

struct Vertex
{
	Vertex();
	Vertex( const aiMesh* a_Mesh, const int a_Index );

	vec4 Position;		// Added to attribute location 0
	vec4 Normal;		// Added to attribute location 1
	vec2 TexCoords[8];	// Added to attribute location 2

	// Normal-Mapping Data
	vec3 Tangent;	// Added to attribute location 3
	vec3 Bitangent;	// Added to attribute location 4

	// Bone indexes which will influence this vertex
	// Added to attribute location 5
	int m_BoneIDs[ MAX_BONE_INFLUENCE ]; 	
	// Weights from each bone
	// Added to attribute location 6
	float m_Weights[ MAX_BONE_INFLUENCE ];

	void SetBoneData( int a_BoneID, float a_Weight );
};

struct BoneInfo
{
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



#pragma region Model

typedef std::map<string, BoneInfo> BoneInfoMap;

class RModel : public RResource
{
	friend class MeshLoader;

public:
	void Draw();

	auto& GetMeshes() { return m_Meshes; }
	auto& GetBoneInfoMap() { return m_BoneInfoMap; }
	auto  GetBoneCount()   { return m_BoneInfoMap.size(); }

protected:
	std::vector<MeshHandle> m_Meshes;
	BoneInfoMap m_BoneInfoMap;
};

using ModelHandle = std::shared_ptr<RModel>;

#pragma endregion



#pragma region Loader

class MeshLoader
{
private:
	MeshLoader();
	~MeshLoader() = default;

	std::map<string, MeshHandle> m_LoadedMeshes;
	std::map<string, ModelHandle> m_LoadedModels;

public:
	MeshLoader( MeshLoader& Other ) = delete;
	void operator=( const MeshLoader& ) = delete;

	static MeshLoader* GetInstance();

	MeshHandle LoadMesh( const aiMesh* a_Mesh );
	MeshHandle GetMesh( const string& a_Name );
	const auto& GetLoadedMeshes() const { return m_LoadedMeshes; }

	ModelHandle LoadModel( const aiScene* a_Scene );
	ModelHandle GetModel( const string& a_Name );
	const auto& GetLoadedModels() const { return m_LoadedModels; }
};

#pragma endregion


