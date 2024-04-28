#pragma once
#include "URenderer.h"

// --- AIE ---
#include "Shader.h"
#include "AIERenderTarget.h"

// --- Engine ---
#include "RMaterial.h"
#include "RMesh.h"


class UMeshRenderer :
    public URenderer
{
public:
    UMeshRenderer(AActor* a_Owner);
    virtual ~UMeshRenderer();

    virtual void Draw() override;

protected:
    aie::OBJMesh* m_OldMesh = nullptr;
    MeshHandle m_Mesh = nullptr;


    virtual void OnEnabled();
    virtual void OnDisabled();

public:
    MeshHandle GetMesh() { return m_Mesh; }
    bool SetMesh(const char* a_MeshName, bool a_LoadTextures = true, bool a_FlipTextureV = false);
    bool SetMesh( const string& a_Name );
    void SetMesh( MeshHandle a_Mesh );

#pragma region ImGui

public:
    virtual void Draw_ImGui();

#pragma endregion
};

