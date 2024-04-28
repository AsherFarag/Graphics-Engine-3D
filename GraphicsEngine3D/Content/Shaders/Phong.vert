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
uniform mat4 ModelMatrix;

void main()
{
      vPosition    = ModelMatrix * Position;
      vNormal      = vec3(Normal.x, Normal.y, Normal.z);//((ModelMatrix) * Normal).xyz; //normalize(inverse(ModelMatrix) * Normal).xyz; 
      vTexCoords   = TexCoords;
      vTangent     = (inverse(ModelMatrix) * vec4(Tangent.xyz, 1)).xyz;
      vBiTangent   = cross(vNormal, vTangent) * Tangent.w;

      gl_Position  = ProjectionViewModel * Position;

      vDepth = gl_Position.z;
}