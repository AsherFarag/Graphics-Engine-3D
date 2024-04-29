#include "Math.h"

mat3 Math::AssimpMatToGLM( const aiMatrix3x3& a_AssimpMat )
{
    return glm::transpose( glm::make_mat3x3( ( a_AssimpMat[0] )));
}

mat4 Math::AssimpMatToGLM( const aiMatrix4x4& a_AssimpMat )
{
    return ( glm::make_mat4x4( ( a_AssimpMat[0] )));
}
