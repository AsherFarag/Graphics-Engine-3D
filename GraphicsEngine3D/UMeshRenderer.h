#pragma once
#include "URenderer.h"

// --- AIE ---
#include "Shader.h"
#include "AIERenderTarget.h"

// --- Engine ---
#include "RMaterial.h"


class UMeshRenderer :
    public URenderer
{
public:
    UMeshRenderer(AActor* a_Owner);
    virtual ~UMeshRenderer();

    virtual void Draw(mat4 a_ProjectionViewMatrix) override;

protected:
    aie::OBJMesh* m_Mesh;


    virtual void OnEnabled();
    virtual void OnDisabled();

public:
    aie::OBJMesh* GetMesh() { return m_Mesh; }
    bool SetMesh(const char* a_MeshName, bool a_LoadTextures = true, bool a_FlipTextureV = false);

#pragma region ImGui

public:
    virtual void Draw_ImGui();

#pragma endregion
};

