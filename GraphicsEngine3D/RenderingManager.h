#pragma once
#include "UBaseObject.h"

// --- STD ---
#include <List>
#include <vector>
using std::list;
using std::vector;

// --- Engine ---
class ALight;
class ACamera;
class URenderer;
class RMaterial;

const int MAX_LIGHTS = 4;

class RenderingManager
{
public:
    RenderingManager();
    ~RenderingManager();

    void Draw();

protected:
    list<URenderer*> m_Renderers;

    void CalculateDrawOrder(list<URenderer*>& OutDrawBuffer, std::list<RMaterial*>& OutMaterialBuffer);

    ACamera* m_RenderCamera;

    int m_NumOfLights = 0;
    vector<ALight*> m_Lights;
    vector<vec3> GetPointLightPositions();
    vector<vec3> GetPointLightColours();

public:
    bool AddRenderer(URenderer* a_Renderer);
    bool RemoveRenderer(URenderer* a_Renderer);

    void SetRenderCamera(ACamera* a_NewRenderCamera);

    bool AddLight(ALight* a_Light);


    // TEMP
public:
    ALight* m_AmbientLight = nullptr;
};

