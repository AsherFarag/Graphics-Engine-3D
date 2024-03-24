#pragma once
#include "UBaseComponent.h"

#include "OBJMesh.h"

// --- Engine ---
#include "RMaterial.h"
class RenderingManager;

class URenderer :
    public UBaseComponent
{
public:
    URenderer(AActor* a_Owner);
    virtual ~URenderer();

    virtual void Draw(mat4 a_ProjectionViewMatrix) = 0;

protected:
    aie::OBJMesh* m_Mesh;
    RMaterial* m_Material = nullptr;

    // Owning Rendering Manager
    RenderingManager* m_RenderingManager;

    virtual void OnEnabled();
    virtual void OnDisabled();

public:
    aie::OBJMesh* GetMesh() { return m_Mesh; }
    bool SetMesh(const char* a_MeshName, bool a_LoadTextures = true, bool a_FlipTextureV = false);

    void SetRenderingManager(RenderingManager* a_RenderingManager);

    RMaterial* GetMaterial() const { return m_Material; }
    void SetMaterial(RMaterial* a_Material) { m_Material = a_Material; }

#pragma region ImGui

public:
    virtual void Draw_ImGui() {};

#pragma endregion
};

