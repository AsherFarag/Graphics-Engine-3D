#include "RenderManager.h"

#include "ACamera.h"

RenderManager::RenderManager()
{
}

RenderManager::~RenderManager()
{
}

void RenderManager::RenderToTarget(RenderTarget* a_Target, ACamera* a_Camera)
{
	#if IS_EDITOR

	m_RenderStats.ResetFrameStats();

	#endif // IS_EDITOR

	RenderBatch Batch;
	RenderPreProcess(a_Camera, Batch);
	Render(a_Camera, Batch);
}

void RenderManager::RenderPreProcess(ACamera* a_Camera, RenderBatch& OutBatch)
{
#if IS_EDITOR

	m_RenderStats.DrawnMaterials = OutBatch.MaterialBatches.size();

#endif // IS_EDITOR
}

void RenderManager::Render(ACamera* a_Camera, const RenderBatch& a_RenderBatch)
{
	mat4 ProjectionViewMatrix = a_Camera->GetProjectionMatrix() * a_Camera->GetViewMatrix();
	for (auto MaterialBatch : a_RenderBatch.MaterialBatches)
	{
		for (auto& MeshBatch : MaterialBatch.MeshBatches)
		{
			for (int i = 0; i < MeshBatch.NumOfInstances; ++i)
			{
				// bind transforms for lighting
				MaterialBatch.Material->m_Shader->bindUniform("ProjectionViewModel", ProjectionViewMatrix * MeshBatch.ModelMatricies[i]);
				MaterialBatch.Material->m_Shader->bindUniform("ModelMatrix", MeshBatch.ModelMatricies[i]);

				MeshBatch.Mesh->draw(false);
			}
		}
	}


#pragma Message("TODO: INSTANCING")
	// --- INSTANCING ---
	//for (auto MaterialBatch : a_RenderBatch.MaterialBatches)
	//{
	//	BindMaterial(MaterialBatch.Material);

	//	MaterialBatch.Material->m_Shader->bindUniform("ProjectionViewModel", a_ProjectionViewMatrix * m_Owner->GetTransform().GetTransform());

	//	for (auto MeshBatch : MaterialBatch.MeshBatches)
	//	{
	//		MeshBatch.Mesh->Draw(MeshBatch.NumOfInstances, &MeshBatch.ModelMatricies[0]);
	//	}
	//}
}

void RenderManager::BindMaterial(RMaterial* a_Material)
{

	int program = -1;
	glGetIntegerv(GL_CURRENT_PROGRAM, &program);

	if (program == -1)
	{
		printf("No shader bound!\n");
		return;
	}

	// Pull Uniforms from the shader
	int kaUniform = glGetUniformLocation(program, "Ka");
	int kdUniform = glGetUniformLocation(program, "Kd");
	int ksUniform = glGetUniformLocation(program, "Ks");
	int keUniform = glGetUniformLocation(program, "Ke");
	int opacityUniform = glGetUniformLocation(program, "Opacity");
	int specPowUniform = glGetUniformLocation(program, "SpecularPower");

	// Pull Texture Uniforms from the shader
	int alphaTexUniform			= glGetUniformLocation(program, "AlphaTexture");
	int ambientTexUniform		= glGetUniformLocation(program, "AmbientTexture");
	int diffuseTexUniform		= glGetUniformLocation(program, "DiffuseTexture");
	int specTexUniform			= glGetUniformLocation(program, "SpecularTexture");
	int specHighlightTexUniform = glGetUniformLocation(program, "SpecularHighlightTexture");
	int normalTexUniform		= glGetUniformLocation(program, "NormalTexture");
	int dispTexUniform			= glGetUniformLocation(program, "DisplacementTexture");

	// bind material
	if (kaUniform >= 0)
		glUniform3fv(kaUniform, 1, &a_Material->Ambient[0]);
	if (kdUniform >= 0)
		glUniform3fv(kdUniform, 1, &a_Material->Diffuse[0]);
	if (ksUniform >= 0)
		glUniform3fv(ksUniform, 1, &a_Material->Specular[0]);
	if (keUniform >= 0)
		glUniform3fv(keUniform, 1, &a_Material->Emissive[0]);
	if (opacityUniform >= 0)
		glUniform1f(opacityUniform, a_Material->Opacity);
	if (specPowUniform >= 0)
		glUniform1f(specPowUniform, a_Material->SpecularPower);

	glActiveTexture(GL_TEXTURE0);
	if (a_Material->DiffuseTexture.getHandle() > 0)
		glBindTexture(GL_TEXTURE_2D, a_Material->DiffuseTexture.getHandle());
	else if (diffuseTexUniform >= 0)
		glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE1);
	if (a_Material->AlphaTexture.getHandle() > 0)
		glBindTexture(GL_TEXTURE_2D, a_Material->AlphaTexture.getHandle());
	else if (alphaTexUniform >= 0)
		glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE2);
	if (a_Material->AmbientTexture.getHandle() > 0)
		glBindTexture(GL_TEXTURE_2D, a_Material->AmbientTexture.getHandle());
	else if (ambientTexUniform >= 0)
		glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE3);
	if (a_Material->SpecularTexture.getHandle() > 0)
		glBindTexture(GL_TEXTURE_2D, a_Material->SpecularTexture.getHandle());
	else if (specTexUniform >= 0)
		glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE4);
	if (a_Material->SpecularHighlightTexture.getHandle() > 0)
		glBindTexture(GL_TEXTURE_2D, a_Material->SpecularHighlightTexture.getHandle());
	else if (specHighlightTexUniform >= 0)
		glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE5);
	if (a_Material->NormalTexture.getHandle() > 0)
		glBindTexture(GL_TEXTURE_2D, a_Material->NormalTexture.getHandle());
	else if (normalTexUniform >= 0)
		glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE6);
	if (a_Material->DisplacementTexture.getHandle() > 0)
		glBindTexture(GL_TEXTURE_2D, a_Material->DisplacementTexture.getHandle());
	else if (dispTexUniform >= 0)
		glBindTexture(GL_TEXTURE_2D, 0);
}
