#pragma once

#pragma region STD

#include <vector>

#pragma endregion


#pragma region Engine

#include "RMesh.h"
#include "OBJMesh.h"
#include "UTransform.h"

// Renderers
#include "URenderer.h"
#include "UMeshRenderer.h"

class RenderTarget;
class ACamera;
class ALight;

#pragma endregion

#if IS_EDITOR

#define DEBUG_RENDER_STAT(Stat, Op, Val) m_RenderStats.Stat Op Val
#define DEBUG_RENDER_STAT_FUNC(Func, Args) m_RenderStats.Func(Args)

#else

#define DEBUG_RENDER_STAT(Stat, Op, Val)
#define DEBUG_RENDER_STAT_FUNC( Func, Args )

#endif // IS_EDITOR


const int MAX_LIGHTS = 4;

class RenderManager
{
	friend class World;
	friend class UMeshRenderer;

public:
	RenderManager();
	~RenderManager();
	
	void Reset();

private:

	std::vector<UMeshRenderer*> m_MeshRenderers;
	std::vector<std::pair<MaterialHandle, MeshHandle>> m_DrawBuffer;

	int m_NumOfLights = 0;
	std::vector<ALight*> m_Lights;

	// --- Render Pipeline ---
	// Generates a render batch that can be rendered
	void RenderPreProcess(ACamera* a_Camera);
	// Binds a material and then batch draws the meshes using that material
	void Render(ACamera* a_Camera);
	// Finished up the Render, such as applying post-processing
	void RenderPostProcess(RenderTarget* a_Target);
	
	ShaderHandle m_ShaderInUse = nullptr;
	MaterialHandle m_MaterialInUse = nullptr;
	void UseMaterial( MaterialHandle a_Material, bool Force = false );

	void BindLights( ShaderHandle a_Shader );
	void BindMaterial( MaterialHandle a_Material ) {}

protected:

	bool AddRenderer(URenderer* a_Renderer);
	bool RemoveRenderer(URenderer* a_Renderer);

	void AddMeshRenderer( UMeshRenderer* a_Renderer );
	void RemoveMeshRenderer( UMeshRenderer* a_Renderer );

	TODO("Fix up light system");
#pragma region  --- Lights --- 

public:
	bool AddLight(ALight* a_Light);
	bool RemoveLight(ALight* a_Light);
	void DrawLightGizmos();

#pragma endregion

#if IS_EDITOR

	struct RenderStats
	{
		// --- Per Frame Stats ---
		unsigned int DrawnMaterials = 0;
		unsigned int DrawnMeshes = 0;
		unsigned int DrawCalls = 0;
		unsigned int Instances = 0;
		float PreProcessTime = 0.f;
		float RenderTime = 0.f;
		float PostProcessTime = 0.f;


		void ResetFrameStats()
		{
			DrawnMaterials = 0;
			DrawnMeshes = 0;
			DrawCalls = 0;
			Instances = 0;
		}

		void PrintStats()
		{
			if ( ImGui::TreeNode( "Render Stats" ) )
			{
				ImGui::Text( "Drawn Materials: %i", DrawnMaterials );
				ImGui::Text( "Drawn Meshes: %i", DrawnMeshes );
				ImGui::Text( "Draw Calls: %i", DrawCalls );
				ImGui::Text( "Instances: %i", Instances );
				ImGui::Text( "Pre-Process Time: %.4fms", PreProcessTime );
				ImGui::Text( "Render Time: %.4fms", RenderTime );
				ImGui::Text( "Post-Process Time: %.4fms", PostProcessTime );

				ImGui::TreePop();
			}
		}
	};

	RenderStats m_RenderStats;

#endif // IS_EDITOR
};

