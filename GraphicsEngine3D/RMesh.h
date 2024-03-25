#pragma once
#include "RResource.h"

// --- STD ---
#include <vector>

// --- Engine ---
#include "RMaterial.h"

#define MESH_FILE_PATH "Mesh/"

class RMesh
	: public RResource
{
public:
	RMesh() {}
	virtual ~RMesh();

	struct Vertex
	{
		vec4 m_Position;	// Added to attribute location 0
		vec4 m_Normal;		// Added to attribute location 1
		vec2 m_TexCoord;	// Added to attribute location 2

		// Normal-Mapping Data
		vec4 m_Tangent;		// Added to attribute location 3
	};

	enum EPrimativeShape
	{
		EPS_Quad,
		EPS_Box,
		EPS_Cylinder,
		EPS_Pyramid,
		EPS_Sphere,
		EPS_Cone,
	};

private:
	struct MeshChunk
	{
		unsigned int m_TriCount = 0;
		unsigned int m_VAO = 0; // The Vertex Array Object
		unsigned int m_VBO = 0; // The Vertex Buffer Object
		unsigned int m_IBO = 0; // The Index Buffer Object

		// Not - Implemented
		unsigned int m_IndexCount;
		unsigned int m_MaterialID;
	};

	MeshChunk m_MeshChunk;
	RMaterial* m_Materials;

public:
	void Initialise(unsigned int a_VertexCount, const Vertex* a_Vertices, unsigned int a_IndexCount = 0, unsigned int* a_Indices = nullptr);

	void Initialise(const char* a_FileName);

	void Initialise(EPrimativeShape a_PrimativeMesh);

	void InitialiseQuad();

	void InitialiseFullScreenQuad();

	// Will fail if it cannot find the file OR a mesh has already been loaded in this instance
	bool Load(const char* a_FileName, bool a_LoadTextures = true, bool a_FlipTextures = false);

	void Draw();
	void BatchDraw(std::vector<mat4> a_MeshTransforms);

	RMaterial* GetMaterial() { return m_Materials; }
	void SetMaterial(RMaterial* a_Material) { m_Materials = a_Material; }

	// Material access
	//size_t GetMaterialCount() const { return m_Materials.size(); }
	//RMaterial& GetMaterial(size_t index = 0) { return m_Materials[index]; }
};

