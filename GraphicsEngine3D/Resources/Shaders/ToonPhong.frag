// A Toon Phong Fragment Shader

#version 410

out vec4 FragColour;

in vec2 vTexCoords;
in vec3 vNormal;
in vec4 vPosition;

uniform vec4 CameraPosition;
uniform vec3 AmbientLight; // Ambient light colour
uniform vec3 AmbientLightDirection; // Ambient light colour

uniform vec3 Ka; // ambient material colour
uniform vec3 Kd; // diffuse material colour
uniform vec3 Ks; // specular material colour
uniform float SpecularPower; // material specular power

uniform sampler2D DiffuseTex;
uniform sampler2D SpecularTexture;


const int MAX_LIGHTS = 4;
uniform int   NumOfLights;
uniform vec3  PointLightColors[MAX_LIGHTS];
uniform vec3  PointLightPositions[MAX_LIGHTS];
uniform float PointLightFallOffs[MAX_LIGHTS];

vec3 Diffuse(vec3 Colour, vec3 Normal, vec3 Direction)
{
      // Colour * Lambert Term
      return Colour * max( 0.0, dot( Normal, -Direction ) );
}

vec3 Specular(vec3 Colour, vec3 Normal, vec3 Direction, vec3 View)
{
      vec3 R = reflect( Direction, Normal );
      float SpecularTerm = pow( max( 0, dot( R, View ) ), SpecularPower );
      return SpecularTerm * Colour;
}

void main()
{
      // Ensure the input Vectors are normalised
      vec3 N = normalize(vNormal);
      //vec3 L = normalize(AmbientLightDirection);

      // Extract values from Texture Coordinates
      vec3 TextureDiffuse  = texture(DiffuseTex,  vTexCoords).rgb;
      vec3 TextureSpecular = texture(SpecularTexture, vTexCoords).rgb;

      // Calculate view vector and reflection vector
      vec3 V = normalize(CameraPosition.xyz - vPosition.xyz);

      vec3 DiffuseTotal;//Diffuse(AmbientLight, N, L);
      vec3 SpecularTotal;//Specular(AmbientLight, N, L, V);

      for (int i = 0; i < NumOfLights; i++)
      {
            vec3 Direction = vPosition.xyz - PointLightPositions[i];
            float Distance = length(Direction);
            Direction = Direction / Distance;

            // Attenuate Light Intensity using the Inverse Square Law
            Distance = Distance * PointLightFallOffs[i];
            vec3 Colour = PointLightColors[i] / (Distance * Distance);

            DiffuseTotal += Diffuse(Colour, N, Direction);
            SpecularTotal += Specular(Colour, N, Direction, V);
      }

      // Add Rim Lighting
      float FresnelTerm = pow( 1.0 - max( 0.0, dot( N, V ) ), 2.5 );

      // Apply Toon Shading
      //FresnelTerm  = round( FresnelTerm * 1.0 ) / 1.0;


      // Calculate Each Colour Property
      vec3 Ambient  = AmbientLight * Ka * TextureDiffuse;
      vec3 Diffuse  = Kd * TextureDiffuse  * DiffuseTotal;
      vec3 Specular = Ks * TextureSpecular * SpecularTotal;

      // Apply Rim Lighting to Ambient
      Ambient += ( FresnelTerm * vec3( 0.1, 0.1, 0.2 ) * 3 );

      Ambient  = round( Ambient * 3.0 ) / 3.0;
      Diffuse  = round( Diffuse * 5.0 ) / 5.0;
      Specular = round( Specular * 1.0 ) / 1.0;

      // Output Colour
      FragColour = vec4( Ambient + Diffuse + Specular, 1 );

      // Test Outputs
      //FragColour = vec4(N, 1 );
}