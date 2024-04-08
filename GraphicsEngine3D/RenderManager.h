#pragma once

#pragma region STD
#include <vector>
#pragma endregion


#pragma region Engine
#include "RenderTarget.h"

#include "RMesh.h"
#include "OBJMesh.h"
#include "UTransform.h"

// Renderers
#include "URenderer.h"
#include "UMeshRenderer.h"

class ACamera;
#pragma endregion

struct MeshBatch
{
	OBJMesh* Mesh;
	unsigned int NumOfInstances;
	mat4* ModelMatricies;
};

struct MaterialBatch
{
	RMaterial* Material;
	std::vector<MeshBatch> MeshBatches;
};

struct RenderBatch
{
	std::vector<MaterialBatch> MaterialBatches;
};

// === Render Manager ===
//	
// 
// 
//


class RenderManager
{
public:
	RenderManager();
	~RenderManager();

	void RenderToTarget(RenderTarget* a_Target, ACamera* a_Camera);

private:

	std::vector<UMeshRenderer> m_MeshRenderers;

private:

	// --- Render Pipeline ---
	// Generates a render batch that can be rendered
	void RenderPreProcess(ACamera* a_Camera, RenderBatch& OutBatch);
	// Binds a material and then batch draws the meshes using that material
	void Render(ACamera* a_Camera, const RenderBatch& a_RenderBatch);
	// Finished up the Render, such as applying post-processing
	void RenderPostProcess(RenderTarget* a_Target) {}



	void BindMaterial(RMaterial* a_Material);

#if IS_EDITOR

	struct RenderStats
	{
		// --- Per Frame Stats
		unsigned int DrawnMaterials = 0;
		unsigned int DrawnMeshes = 0;
		unsigned int DrawCalls = 0;
		unsigned int Instances = 0;


		void ResetFrameStats()
		{
			DrawnMaterials = 0;
			DrawnMeshes = 0;
			DrawCalls = 0;
			Instances = 0;
		}

		void PrintStats()
		{
			ImGui::Text("Drawn Materials: %i", DrawnMaterials);
			ImGui::Text("Drawn Meshes: %i", DrawnMeshes);
			ImGui::Text("Draw Calls: %i", DrawCalls);
			ImGui::Text("Instances: %i", Instances);
		}
	};

	RenderStats m_RenderStats;

#endif // IS_EDITOR

};

