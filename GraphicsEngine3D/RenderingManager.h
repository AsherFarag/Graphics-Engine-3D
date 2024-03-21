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

    bool StartUp();
    void Draw();
    bool End();

protected:
    list<URenderer*> m_Renderers;

    void CalculateDrawOrder(list<URenderer*>& OutDrawBuffer, std::list<RMaterial*>& OutMaterialBuffer);

    ACamera* m_RenderCamera;

    #pragma region  --- Lights --- 

protected:
    int m_NumOfLights = 0;
    vector<ALight*> m_Lights;
    ALight* m_AmbientLight = nullptr;

    int GetActiveNumOfLights();
    vector<vec3> GetPointLightPositions();
    vector<vec3> GetPointLightColours();
    vector<float> GetPointLightFallOffs();

public:
    bool AddLight(ALight* a_Light);
    bool RemoveLight(ALight* a_Light);

    void SetAmbientLight(ALight* a_AmbientLight);

    #pragma endregion

public:
    bool AddRenderer(URenderer* a_Renderer);
    bool RemoveRenderer(URenderer* a_Renderer);

    void SetRenderCamera(ACamera* a_NewRenderCamera);


    // TEMP
public:


};

