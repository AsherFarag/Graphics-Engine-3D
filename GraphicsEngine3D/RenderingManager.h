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
class UMeshRenderer;
class RMaterial;

const int MAX_LIGHTS = 4;

// === Rendering Manager ===
//
// Only one Rendering Manager exists per world.
// It handles and stores references to all Renderers, Lights and Cameras.
//  
// TODO:
// --- Batch Rendering ---
// It converts all render data into batches for the camera.
// Such as, getting all mesh renderers using the same mesh and passing only the mesh and the renderer transforms, as an array, to the camera.
//

class RenderingManager
{
public:
    RenderingManager();
    ~RenderingManager();

    bool StartUp();
    void Draw();
    bool End();

    void Render(ACamera* Camera, mat4 ProjectedView);
    void DrawShadows(ACamera* Camera, mat4 ProjectedView);


#if IS_EDITOR

    void DrawViewports();

#endif // IS_EDITOR

protected:
    list<URenderer*> m_Renderers;
    list<UMeshRenderer*> m_MeshRenderers;

    void CalculateDrawOrder(list<URenderer*>& OutDrawBuffer, std::list<RMaterial*>& OutMaterialBuffer);

    vector<ACamera*> m_RenderCameras;

    #pragma region  --- Lights --- 

protected:
    int m_NumOfLights = 0;
    vector<ALight*> m_Lights;
    ALight* m_AmbientLight = nullptr;

    int GetActiveNumOfLights();
    vector<vec3> GetPointLightPositions();
    vector<vec3> GetPointLightColours();
    vector<float> GetPointLightFallOffs();

    bool AddRenderer(URenderer* a_Renderer);
    bool RemoveRenderer(URenderer* a_Renderer);

public:
    bool AddLight(ALight* a_Light);
    bool RemoveLight(ALight* a_Light);

    void SetAmbientLight(ALight* a_AmbientLight);

    #pragma endregion

public:
    bool AddMeshRenderer(UMeshRenderer* a_Renderer);
    bool RemoveMeshRenderer(UMeshRenderer* a_Renderer);

    bool AddRenderCamera(ACamera* a_NewRenderCamera);
    bool RemoveRenderCamera(ACamera* a_NewRenderCamera);

    aie::ShaderProgram* m_ShadowProgram;

};

