#version 410

layout( location = 0 ) in vec4 Position;
layout( location = 1 ) in vec4 Normal;
layout( location = 2 ) in vec2 TexCoords;
layout( location = 3 ) in vec4 Tangent;
layout( location = 5 ) in ivec4 BoneIds;
layout( location = 6 ) in vec4 Weights;

out vec4 vPosition;
out vec3 vNormal;
out vec2 vTexCoords;
out vec3 vTangent;
out vec3 vBiTangent;
out float vDepth;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 FinalBonesMatrices[MAX_BONES];

uniform mat4 ProjectionViewModel;
uniform mat4 ModelMatrix;

void main()
{
        //vPosition    = ModelMatrix * Position;
        vNormal      = vec3(Normal.x, Normal.y, Normal.z);//((ModelMatrix) * Normal).xyz; //normalize(inverse(ModelMatrix) * Normal).xyz; 
        vTexCoords   = TexCoords;
        vTangent     = (inverse(ModelMatrix) * vec4(Tangent.xyz, 1)).xyz;
        vBiTangent   = cross(vNormal, vTangent) * Tangent.w;


        vec4 totalPosition = vec4(0.f);
        vec3 totalNormal = vec3(0.f);

        for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
        {
            int BoneIndex = BoneIds[i];
            if (BoneIndex < 0)
            {
                //totalPosition = Position;
                continue;
            }

            float Weight = Weights[i];
            mat4 BoneTransform = FinalBonesMatrices[BoneIndex];
            vec4 InfluencedVertexPosition = BoneTransform * Position;
            vec4 Influence = InfluencedVertexPosition * Weight;
            totalPosition += Influence;
            totalNormal += (FinalBonesMatrices[BoneIndex] * Normal).xyz * Weight;
        }
        if (totalPosition == vec4(0.f))
        {
            totalPosition = Position;
        }

        vPosition = ModelMatrix * totalPosition;
        vNormal = Normal.xyz;//(ModelMatrix * vec4(totalNormal, 1.f)).xyz;
        gl_Position = ProjectionViewModel * totalPosition;

        // Skeleton is a chain of matrices from origin to a bone
        // Each matrix is how that bone is oriented in relation to its parent.
        // Multiply all of those transforms together to get a matrix that is in relation to model space.

        // Tb0 * Tb1 * .... Tbn = BindPostTransform.
        // BineposeTransform takes a Bone space vertex into model space.

        // The inverse of BindposeTransform = BPT-1
        // I can use this to bring a model space vertex into bone space.
        // BoneSpaceVertex = BindposeTransform * ModelSpaceVertex 
        // Tnb0 * Tnb1 * Tnb2 * ... * Tnbn = NewBoneTransform.

        // Get the new ModelSpace vertex
        // NewBoneTransform * BoneSpaceVertex = NewModelSpaceVertex

        // Final ModelSpaceVertex = NewModelSpaceVertex0 * Weight0 + ... + NewModelSpaceVertexN * WeightN

        // ProjectionViewModel * FinalModelSpaceVertex = WorldSpaceVertex


        
        vDepth = gl_Position.z;
}