#pragma once
#include "URenderer.h"

// --- AIE ---
#include "Shader.h"

// --- Engine ---
#include "Material.h"
#include "Mesh.h"


class UMeshRenderer : public URenderer
{
public:
    UMeshRenderer(AActor* a_Owner);
    virtual ~UMeshRenderer();

    virtual void Draw() override;

private:
    MeshHandle m_Mesh = nullptr;
    MaterialHandle m_OverrideMaterial = nullptr;

    virtual void OnEnabled() {};
    virtual void OnDisabled() {};

public:
    MaterialHandle GetMaterial();
    void SetMaterial( MaterialHandle a_Material );
    MeshHandle GetMesh();
    void SetMesh( MeshHandle a_Mesh );

#pragma region ImGui

public:
    virtual void Draw_ImGui();

#pragma endregion
};

