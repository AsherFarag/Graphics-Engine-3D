#version 410

out vec4 FragColour;

in vec2 vTexCoords;
in vec3 vNormal;
in vec3 vTangent;
in vec3 vBiTangent;
in vec4 vPosition;

uniform sampler2D DiffuseTex;

void main()
{
      // Output Colour
      FragColour = vec4(1);
}