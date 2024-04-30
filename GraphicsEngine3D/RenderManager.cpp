#include "RenderManager.h"

#include <algorithm>

#include "ACamera.h"
#include "RMaterialInstance.h"
#include "ALight.h"
#include "RenderTarget.h"
#include "UAnimatorComponent.h"

RenderManager::RenderManager()
{
}

RenderManager::~RenderManager()
{
}

void RenderManager::Reset()
{
	m_MeshRenderers.clear();
	m_DrawBuffer.clear();

	m_ShaderInUse = nullptr;
	m_MaterialInUse = nullptr;
	m_MaterialInstanceInUse = nullptr;

	m_NumOfLights = 0;
	m_Lights.clear();
}

void RenderManager::RenderPreProcess( ACamera* a_Camera )
{
	DEBUG_RENDER_STAT_FUNC( ResetFrameStats );

	DEBUG_TIME_DECLARE;
	DEBUG_TIME_BEGIN;

	m_DrawBuffer.clear();

	// Step 1:
	// Get all objects that should be rendered by the camera
	NOT_IMPLEMENTED;
	m_DrawBuffer.resize( m_MeshRenderers.size() );
	for ( int i = 0; i < m_DrawBuffer.size(); ++i )
	{
		m_DrawBuffer[i] = m_MeshRenderers[i];
	}


	// Step 2:
	// Sort objects by Shader Program
	// Potentially implement an option to change sort type. E.g. By Mesh or by Shader
	//std::sort( m_DrawBuffer.begin(), m_DrawBuffer.end(),
	//	[&]( const UMeshRenderer& a, const UMeshRenderer& b )
	//	{
	//		return a.GetMaterial()->GetMaster().get() < b.GetMaterial()->GetMaster().get();
	//	} );

	// Step 3:
	// Figure out how to do instancing later

	a_Camera->BeginRender();

	DEBUG_RENDER_STAT( PreProcessTime, =, DEBUG_TIME_END( 1000.f ) );
}

void RenderManager::Render( ACamera* a_Camera )
{
	DEBUG_TIME_DECLARE;
	DEBUG_TIME_BEGIN;

	mat4 ProjectionViewMatrix = a_Camera->GetProjectionViewMatrix();

	for ( unsigned int i = 0; i < m_DrawBuffer.size(); ++i )
	{
		if ( m_DrawBuffer[ i ]->GetMaterial() == nullptr )
			continue;

		UseMaterialInstance( m_DrawBuffer[ i ]->GetMaterial() );

		auto& ModelMatrix = m_DrawBuffer[ i ]->GetOwner()->GetTransform().GetTransformation();
		m_ShaderInUse->bindUniform( "ProjectionViewModel", ProjectionViewMatrix * ModelMatrix );
		m_ShaderInUse->bindUniform( "ModelMatrix", ModelMatrix );

		// ===== Animation Data =====
		if ( auto animator = m_DrawBuffer[ i ]->GetOwner()->GetComponent<UAnimatorComponent>(); animator->GetPose().size() > 0 )
		{
			std::vector<glm::mat4> Transforms = animator->GetPose();

			auto Skeleton = animator->GetSkeleton();
			auto& BindPoseBones = Skeleton->GetBones();
			std::vector<glm::mat4> InverseBindPoseTransforms( BindPoseBones.size() );

			// Create an array of transforms that will take a model space vertex into local bone space for each of the bones IN BIND POSE.
			for ( int i = 0; i < BindPoseBones.size(); ++i )
			{
				InverseBindPoseTransforms[ i ] = ( BindPoseBones[ i ].OffsetMatrix );
			}

			// Take MeshSpaceVertex into BindPoseBoneSpace
			// Vlocal = InverseBindPoseTransforms[i] * Vmesh
			
			// Take BindPoseBoneSpaceVertex into MeshSpace after animation has occurred.
			// Vfinal = Transforms[i] * Vlocal

			// Vfinal = Transforms[i] * (Vlocal)
			// Vfinal = Transforms[i] * InverseBindPoseTransforms[i] * Vmesh

			for ( int i = 0; i < Transforms.size(); ++i )
			{
				Transforms[ i ] = Transforms[ i ] * InverseBindPoseTransforms[ i ];
				//if ( BindPoseBones[ i ].Parent >= 0 )
				//{
				//	vec3 p0 = Transforms[ BindPoseBones[ i ].Parent ][ 3 ];
				//	aie::Gizmos::addLine( p0, Transforms[ i ][ 3 ], vec4( 0,1,0,1 ) );
				//}
				//else
				//{
				//	aie::Gizmos::addSphere( Transforms[ i ][ 3 ], 0.04f, 5, 5, vec4( 0, 1, 1, 1 ) );
				//	continue;
				//}
				//aie::Gizmos::addSphere( Transforms[ i ][ 3 ], 0.04f, 5, 5, vec4( 0, 0, 1, 1 ) );
			}

			m_ShaderInUse->bindUniform( "FinalBonesMatrices", animator->GetPose().size(), &Transforms[ 0 ] );
		}
			

		m_DrawBuffer[ i ]->Draw();
	}

	TODO( "Implement Instancing" );
	// --- INSTANCING ---
	//for (auto MaterialBatch : a_RenderBatch.MaterialBatches)
	//{
	//	BindMaterial(MaterialBatch.Material);
	//
	//	MaterialBatch.Material->m_Shader->bindUniform("ProjectionViewModel", a_ProjectionViewMatrix * m_Owner->GetTransform().GetTransform());
	//
	//	for (auto MeshBatch : MaterialBatch.MeshBatches)
	//	{
	//		MeshBatch.Mesh->Draw(MeshBatch.NumOfInstances, &MeshBatch.ModelMatricies[0]);
	//	}
	//}

	Gizmos::draw( ProjectionViewMatrix );

	a_Camera->EndRender();

	m_ShaderInUse = nullptr;
	m_MaterialInUse = nullptr;
	m_MaterialInstanceInUse = nullptr;

	DEBUG_RENDER_STAT( RenderTime, =, DEBUG_TIME_END( 1000.f ) );
}

void RenderManager::RenderPostProcess( RenderTarget* a_Target )
{
	DEBUG_TIME_DECLARE;
	DEBUG_TIME_BEGIN;

	DEBUG_RENDER_STAT( PostProcessTime, =, DEBUG_TIME_END( 1000.f ) );
}

void RenderManager::UseMaterial( MaterialHandle a_Material, bool Force )
{
	if ( a_Material == m_MaterialInUse && !Force)
		return;

	m_MaterialInUse = a_Material;

	// If this is a different shader, 
	// then use the shader program
	if ( m_MaterialInUse->m_Shader != m_ShaderInUse || Force )
	{
		m_ShaderInUse = m_MaterialInUse->m_Shader;
		m_ShaderInUse->bind();

		if ( m_ShaderInUse->UsesLights() )
			BindLights( m_ShaderInUse );

		return;
	}

	// Bind Material uniforms
	NOT_IMPLEMENTED;
}

void RenderManager::UseMaterialInstance( MaterialInstanceHandle a_MaterialInstance )
{
	if ( a_MaterialInstance == m_MaterialInstanceInUse )
		return;

	m_MaterialInstanceInUse = a_MaterialInstance;

	UseMaterial( a_MaterialInstance->GetMaster() );

	m_MaterialInstanceInUse->Bind();

}

void RenderManager::BindLights( ShaderHandle a_Shader )
{
	int NumOfLights = 0;
	vector<vec3>  LightPositions;
	vector<vec3>  LightColours;
	vector<float> LightFallOffs;

	vec3 AmbientLight(1,0,0);

	for ( auto Light : m_Lights )
	{
		if ( !Light->IsEnabled() )
			continue;
		
		if ( Light->IsAmbient() )
		{
			AmbientLight += Light->GetColour();
		}
		else
		{
			LightPositions.push_back( Light->GetActorPosition() );
			LightColours.push_back( Light->GetColour() );
			LightFallOffs.push_back( Light->GetFallOff() );
			NumOfLights++;
		}
		
	}

	a_Shader->bindUniform( "AmbientLight", AmbientLight );

	a_Shader->bindUniform( "NumOfLights", NumOfLights );
	if ( NumOfLights > 0 )
	{
		a_Shader->bindUniform( "PointLightColors", NumOfLights, &LightColours[ 0 ] );
		a_Shader->bindUniform( "PointLightPositions", NumOfLights, &LightPositions[ 0 ] );
		a_Shader->bindUniform( "PointLightFallOffs", NumOfLights, &LightFallOffs[ 0 ] );
	}
}

void RenderManager::BindMaterial( MaterialHandle a_Material )
{

	//int program = -1;
	//glGetIntegerv(GL_CURRENT_PROGRAM, &program);

	//if (program == -1)
	//{
	//	printf("No shader bound!\n");
	//	return;
	//}

	//// Pull Uniforms from the shader
	//int kaUniform = glGetUniformLocation(program, "Ka");
	//int kdUniform = glGetUniformLocation(program, "Kd");
	//int ksUniform = glGetUniformLocation(program, "Ks");
	//int keUniform = glGetUniformLocation(program, "Ke");
	//int opacityUniform = glGetUniformLocation(program, "Opacity");
	//int specPowUniform = glGetUniformLocation(program, "SpecularPower");

	//// Pull Texture Uniforms from the shader
	//int alphaTexUniform			= glGetUniformLocation(program, "AlphaTexture");
	//int ambientTexUniform		= glGetUniformLocation(program, "AmbientTexture");
	//int diffuseTexUniform		= glGetUniformLocation(program, "DiffuseTexture");
	//int specTexUniform			= glGetUniformLocation(program, "SpecularTexture");
	//int specHighlightTexUniform = glGetUniformLocation(program, "EmissiveTexture");
	//int normalTexUniform		= glGetUniformLocation(program, "NormalTexture");
	//int dispTexUniform			= glGetUniformLocation(program, "DisplacementTexture");

	//// bind material
	//if (kaUniform >= 0)
	//	glUniform3fv(kaUniform, 1, &a_Material->Ambient[0]);
	//if (kdUniform >= 0)
	//	glUniform3fv(kdUniform, 1, &a_Material->Diffuse[0]);
	//if (ksUniform >= 0)
	//	glUniform3fv(ksUniform, 1, &a_Material->Specular[0]);
	//if (keUniform >= 0)
	//	glUniform3fv(keUniform, 1, &a_Material->Emissive[0]);
	//if (opacityUniform >= 0)
	//	glUniform1f(opacityUniform, a_Material->Opacity);
	//if (specPowUniform >= 0)
	//	glUniform1f(specPowUniform, a_Material->SpecularPower);

	//glActiveTexture(GL_TEXTURE0);
	//if (a_Material->DiffuseTexture.getHandle() > 0)
	//	glBindTexture(GL_TEXTURE_2D, a_Material->DiffuseTexture.getHandle());
	//else if (diffuseTexUniform >= 0)
	//	glBindTexture(GL_TEXTURE_2D, 0);

	//glActiveTexture(GL_TEXTURE1);
	//if (a_Material->AlphaTexture.getHandle() > 0)
	//	glBindTexture(GL_TEXTURE_2D, a_Material->AlphaTexture.getHandle());
	//else if (alphaTexUniform >= 0)
	//	glBindTexture(GL_TEXTURE_2D, 0);

	//glActiveTexture(GL_TEXTURE2);
	//if (a_Material->AmbientTexture.getHandle() > 0)
	//	glBindTexture(GL_TEXTURE_2D, a_Material->AmbientTexture.getHandle());
	//else if (ambientTexUniform >= 0)
	//	glBindTexture(GL_TEXTURE_2D, 0);

	//glActiveTexture(GL_TEXTURE3);
	//if (a_Material->SpecularTexture.getHandle() > 0)
	//	glBindTexture(GL_TEXTURE_2D, a_Material->SpecularTexture.getHandle());
	//else if (specTexUniform >= 0)
	//	glBindTexture(GL_TEXTURE_2D, 0);

	//glActiveTexture(GL_TEXTURE4);
	//if (a_Material->EmissiveTexture.getHandle() > 0)
	//	glBindTexture(GL_TEXTURE_2D, a_Material->EmissiveTexture.getHandle());
	//else if (specHighlightTexUniform >= 0)
	//	glBindTexture(GL_TEXTURE_2D, 0);

	//glActiveTexture(GL_TEXTURE5);
	//if (a_Material->NormalTexture.getHandle() > 0)
	//	glBindTexture(GL_TEXTURE_2D, a_Material->NormalTexture.getHandle());
	//else if (normalTexUniform >= 0)
	//	glBindTexture(GL_TEXTURE_2D, 0);

	//glActiveTexture(GL_TEXTURE6);
	//if (a_Material->DisplacementTexture.getHandle() > 0)
	//	glBindTexture(GL_TEXTURE_2D, a_Material->DisplacementTexture.getHandle());
	//else if (dispTexUniform >= 0)
	//	glBindTexture(GL_TEXTURE_2D, 0);
}

bool RenderManager::AddRenderer( URenderer* a_Renderer )
{
	return false;
}

bool RenderManager::RemoveRenderer( URenderer* a_Renderer )
{
	return false;
}

void RenderManager::AddMeshRenderer( UMeshRenderer* a_Renderer )
{
	if ( std::find( m_MeshRenderers.begin(), m_MeshRenderers.end(), a_Renderer ) != m_MeshRenderers.end() )
	{
		LOG( Default, "Couldn't add mesh renderer to m_MeshRenders as it already exists inside m_MeshRenders" );
		return;
	}

	m_MeshRenderers.push_back( a_Renderer );
}

void RenderManager::RemoveMeshRenderer( UMeshRenderer* a_Renderer )
{
	auto i = std::find( m_MeshRenderers.begin(), m_MeshRenderers.end(), a_Renderer );
	if ( i != m_MeshRenderers.end() )
		m_MeshRenderers.erase( i );
}

bool RenderManager::AddLight( ALight* a_Light )
{
	m_Lights.push_back( a_Light );
	return true;
}

bool RenderManager::RemoveLight( ALight* a_Light )
{
	return false;
}

void RenderManager::DrawLightGizmos()
{
	for ( auto& light : m_Lights )
	{
		if ( light->IsEnabled() )
		{
			aie::Gizmos::addSphere( light->GetActorPosition(), 0.05f, 5, 5, vec4( light->GetColour(), 1 ) );
		}
	}
}
