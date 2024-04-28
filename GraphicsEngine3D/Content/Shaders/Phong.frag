#version 410

out vec4 FragColour;

in vec2 vTexCoords;
in vec3 vNormal;
in vec3 vTangent;
in vec3 vBiTangent;
in vec4 vPosition;

uniform vec4 CameraPosition;
uniform vec3 AmbientLight; // Ambient light colour
uniform vec3 AmbientLightDirection; // Ambient light colour

uniform vec3 Ka; // ambient material colour
uniform vec3 Kd; // diffuse material colour
uniform vec3 Ks; // specular material colour
uniform float SpecularPower; // material specular power

uniform sampler2D DiffuseTex;
uniform sampler2D AlphaTex;
uniform sampler2D AmbientTex;
uniform sampler2D SpecularTexture;
uniform sampler2D SpecularHighlightTex;
uniform sampler2D NormalTexture;
uniform sampler2D DisplacementTex;

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
      vec3 T = normalize(vTangent);
      vec3 B = normalize(vBiTangent);
      vec3 N = normalize(vNormal);

      // Extract values from Texture Coordinates
      vec3 TextureDiffuse  = texture(DiffuseTex,  vTexCoords).rgb;
      vec3 TextureSpecular = texture(SpecularTexture, vTexCoords).rgb;
      vec3 TextureNormal   = texture(NormalTexture,   vTexCoords).rgb;

      // Calculate view vector and reflection vector
      vec3 V = normalize(CameraPosition.xyz - vPosition.xyz);

      // Construct Tangent Basis Matrix
      mat3 TBN = mat3(T, B, N);

      // Calculate the Modified Normal from the Texture with the Local Tangent Basis Matrix
      N = TBN * (TextureNormal * 2 - 1);

      vec3 DiffuseTotal;
      vec3 SpecularTotal;

      for (int i = 0; i < NumOfLights; i++)
      {
            vec3 Direction = vPosition.xyz - PointLightPositions[i];
            float Distance = length(Direction);
            Direction = Direction / Distance;

            // Attenuate Light Intensity using the Inverse Square Law
            Distance = Distance * PointLightFallOffs[i];
            vec3 Colour = PointLightColors[i] / (Distance);

            DiffuseTotal += Diffuse(Colour, N, Direction);
            SpecularTotal += Specular(Colour, N, Direction, V);
      }

      // Calculate Each Colour Property
      vec3 Ambient  = AmbientLight * Ka * TextureDiffuse;
      vec3 Diffuse  = Kd * TextureDiffuse  * DiffuseTotal;
      vec3 Specular = Ks * TextureSpecular * SpecularTotal;

      // Output Colour
      FragColour = vec4( Ambient + Diffuse + Specular, 1 );
      //FragColour = vec4(vec3(gl_FragCoord.z), 1.0);

      // Test Outputs
      //FragColour = vec4(N, 1 );
      //FragColour = vec4((TextureNormal), 1);
      //FragColour = vec4(texture(DiffuseTex,   vTexCoords).rgb, 1);
      //FragColour = vec4(vec2(vTexCoords), 0, 1);
}