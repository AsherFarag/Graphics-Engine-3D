#pragma once
#include "UBaseComponent.h"

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
    RMaterial* m_Material = nullptr;

    // Owning Rendering Manager
    RenderingManager* m_RenderingManager;

    virtual void OnEnabled();
    virtual void OnDisabled();

public:
    void SetRenderingManager(RenderingManager* a_RenderingManager);

    RMaterial* GetMaterial() const { return m_Material; }
    void SetMaterial(RMaterial* a_Material) { m_Material = a_Material; }

#pragma region ImGui

public:
    virtual void Draw_ImGui() {};

#pragma endregion
};

