#pragma once
#include "AActor.h"

// --- Engine ---
#include "UMeshRenderer.h"

class AStaticMesh :
    public AActor
{
public:
	enum EPrimitiveMesh
	{
		EPM_Box,
		EPM_Cylinder,
		EPM_Pyramid,
		EPM_Sphere,
		EPM_Cone,
	};


	AStaticMesh();
	AStaticMesh(EPrimitiveMesh a_Primitive);
	AStaticMesh(const char* a_FilePath, bool a_LoadTextures = true, bool a_FlipTextureV = false);
	virtual ~AStaticMesh();

	void Draw() { m_MeshRenderer->Draw(); }

protected:
    UMeshRenderer* m_MeshRenderer;

public:
	UMeshRenderer* GetModel() const { return m_MeshRenderer; }
};

