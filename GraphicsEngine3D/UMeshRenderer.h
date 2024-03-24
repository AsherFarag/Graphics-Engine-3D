#pragma once
#include "URenderer.h"

// --- AIE ---
#include "Shader.h"
#include "RenderTarget.h"

// --- Engine ---
#include "RMaterial.h"


class UMeshRenderer :
    public URenderer
{
public:
    UMeshRenderer(AActor* a_Owner);
    virtual ~UMeshRenderer();

    virtual void Draw(mat4 a_ProjectionViewMatrix) override;

public:

#pragma region ImGui

public:
    virtual void Draw_ImGui();

#pragma endregion
};

