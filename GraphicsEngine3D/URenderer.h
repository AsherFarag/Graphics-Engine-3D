#pragma once
#include "UBaseComponent.h"

#include "OBJMesh.h"

// --- Engine ---
#include "RMaterialInstance.h"
class RenderingManager;

class URenderer :
    public UBaseComponent
{
public:
    URenderer(AActor* a_Owner);
    virtual ~URenderer();

    virtual void Draw(mat4 a_ProjectionViewMatrix) = 0;

protected:
    MaterialInstanceHandle m_Material = nullptr;

    // Owning Rendering Manager
    RenderingManager* m_RenderingManager;

    virtual void OnEnabled();
    virtual void OnDisabled();

public:
    void SetRenderingManager(RenderingManager* a_RenderingManager);

    MaterialInstanceHandle GetMaterial() const { return m_Material; }
    void SetMaterial( MaterialInstanceHandle a_Material) { m_Material = a_Material; }

#pragma region ImGui

public:
    virtual void Draw_ImGui() {};

#pragma endregion
};

