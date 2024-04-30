#include "Math.h"

vec2 Math::AssimpVecToGLM( const aiVector2D& a_AssimpVec )
{
    return vec2( a_AssimpVec.x, a_AssimpVec.y );
}
vec3 Math::AssimpVecToGLM( const aiVector2D& a_AssimpVec, float a_ZElement )
{
    return vec3( a_AssimpVec.x, a_AssimpVec.y, a_ZElement );
}
vec3 Math::AssimpVecToGLM( const aiVector3D& a_AssimpVec )
{
    return vec3( a_AssimpVec.x, a_AssimpVec.y, a_AssimpVec.z );
}

vec4 Math::AssimpVecToGLM( const aiVector3D& a_AssimpVec, float a_WElement )
{
    return vec4( a_AssimpVec.x, a_AssimpVec.y, a_AssimpVec.z, a_WElement );
}

mat3 Math::AssimpMatToGLM( const aiMatrix3x3& a_AssimpMat )
{
    return glm::transpose( glm::make_mat3x3( ( a_AssimpMat[0] )));
}

mat4 Math::AssimpMatToGLM( const aiMatrix4x4& a_AssimpMat )
{
    return ( glm::make_mat4x4( ( a_AssimpMat[0] )));
}
