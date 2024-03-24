// A Toon Phong Fragment Shader
#version 410

in vec4 vPosition;
in vec3 vNormal;
in vec2 vTexCoords;

uniform vec4 CameraPosition;

uniform vec3 AmbientColour; // Ambient light colour
uniform vec3 LightColour;   // Light colour
uniform vec3 LightDirection;

uniform vec3 Ka; // ambient material colour
uniform vec3 Kd; // diffuse material colour
uniform vec3 Ks; // specular material colour
uniform float SpecularPower; // material specular power

uniform sampler2D DiffuseTex;

out vec4 FragColour;

void main()
{
      // ensure normal and light direction are normalised
      vec3 N = normalize(vNormal);
      vec3 L = normalize(LightDirection);

      // Calculate Lambert Term (negate light direction)
      float LambertTerm = max( 0.0, dot( N, L ) );

      // Calculate view vector and reflection vector
      vec3 V = normalize(CameraPosition.xyz - vPosition.xyz);
      vec3 R = normalize(reflect( -L, N ));

      // Calculate specular term
      float SpecularTerm = max( 0.0, pow( max( 0.0, dot( R, V ) ), SpecularPower ) );

      vec3 TextureColour = texture(DiffuseTex, vTexCoords).rgb;

      // Add Rim Lighting
      float FresnelTerm = pow( 1.0 - max( 0.0, dot( N, V ) ), 2.5 );

      // Apply Toon Shading
      FresnelTerm  = round( FresnelTerm * 1.0 ) / 1.0;
      LambertTerm  = round( LambertTerm * 2.0 ) / 2.0;
      SpecularTerm = round( SpecularTerm * 1.0 ) / 1.0;

      // Calculate Each Colour Property
      vec3 Ambient  = AmbientColour * Ka * TextureColour;
      vec3 Diffuse  = LightColour   * Kd * LambertTerm  * TextureColour;
      vec3 Specular = LightColour   * Ks * SpecularTerm * LambertTerm;
      
      // Apply Rim Lighting to Ambient
      Ambient += ( FresnelTerm * vec3( 0.1, 0.1, 0.2 ) );

      // output lambert as grayscale
      FragColour = vec4( Ambient + Diffuse + Specular, 1 );
}