#version 410

layout( location = 0 ) in vec4 Position;
layout( location = 1 ) in vec4 Normal;
layout( location = 2 ) in vec2 TexCoords;
layout( location = 3 ) in vec4 Tangent;

out vec4 vPosition;
out vec3 vNormal;
out vec2 vTexCoords;
out vec3 vTangent;
out vec3 vBiTangent;
out float vDepth;

uniform mat4 ProjectionViewModel;

// Matrix for transforming the Normal
uniform mat4 ModelMatrix;

void main()
{
      vPosition    = ModelMatrix * Position;
      vNormal      = ((ModelMatrix) * Normal).xyz; //(ModelMatrix * Normal).xyz; 
      vTexCoords   = TexCoords;
      vTangent     = (inverse(ModelMatrix) * vec4(Tangent.xyz,0)).xyz;
      vBiTangent   = cross(vNormal, vTangent) * Tangent.w;

      gl_Position  = ProjectionViewModel * Position;

      vDepth = gl_Position.z;
}