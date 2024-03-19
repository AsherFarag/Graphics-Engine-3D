#pragma once
#include "Manager.h"
#include "Renderer2D.h"

class Renderer;

class RenderingManager :
    public Manager
{
public:
    RenderingManager() {}
    ~RenderingManager() {}

    virtual void Init() override;
    virtual void Update(float DeltaTime) override;
    virtual void Draw() override;

    void AddRenderer(Renderer* a_Component);
    void RemoveRenderer(Renderer* a_Component);

    aie::Renderer2D* GetRenderer2D() { return m_Renderer2D; }

protected:
    std::vector<Renderer*> m_Renderers;

    aie::Renderer2D* m_Renderer2D;
};

