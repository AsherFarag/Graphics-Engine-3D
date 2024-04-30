#pragma once
// --- GLM ---
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include "glm/common.hpp"
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/geometric.hpp>
#include "glm/ext.hpp"
#include "glm/fwd.hpp"
#include "glm/trigonometric.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat2;
using glm::mat3;
using glm::mat4;
using glm::quat;

// --- Assimp ---
#include "assimp/matrix3x3.h"
#include "assimp/matrix4x4.h"

namespace Math
{
	vec2 AssimpVecToGLM( const aiVector2D& a_AssimpVec );
	vec3 AssimpVecToGLM( const aiVector2D& a_AssimpVec, float a_ZElement );
	vec3 AssimpVecToGLM( const aiVector3D& a_AssimpVec );
	vec4 AssimpVecToGLM( const aiVector3D& a_AssimpVec, float a_WElement );

	mat3 AssimpMatToGLM( const aiMatrix3x3& a_AssimpMat );
	mat4 AssimpMatToGLM( const aiMatrix4x4& a_AssimpMat );
}

