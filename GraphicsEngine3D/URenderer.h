#pragma once
#include "UBaseComponent.h"

// --- Engine ---
#include "UTransform.h"
class RenderingManager;

class URenderer : public UBaseComponent
{
public:
    URenderer(AActor* a_Owner);
    virtual ~URenderer();

    virtual void Draw() = 0;

protected:
    UTransform m_Transform;
    RenderingManager* m_RenderingManager = nullptr;

    virtual void OnEnabled() {};
    virtual void OnDisabled() {};

public:
    auto& GetTransform() { return m_Transform; }
    auto& GetPos()       { return m_Transform.GetPosition(); }
    auto& GetRot()       { return m_Transform.GetRotation(); }
    auto& GetScale()     { return m_Transform.GetScale(); }

    void SetRenderingManager(RenderingManager* a_RenderingManager);

#pragma region ImGui

public:
    virtual void Draw_ImGui() {};

#pragma endregion
};

