// post-processing fragment shader
#version 410

in vec2 vTexCoord;

uniform sampler2D colourTarget;
uniform int PostProcess;
uniform float ProgressPercent;
uniform int ToonScale;

out vec4 FragColour;

// just output the colour unchanged
vec4 Default(vec2 texCoord)
{
    return texture( colourTarget, texCoord );
}

// simple box blur
vec4 BoxBlur(vec2 texCoord)
{
    vec2 texel = 1.0f / textureSize(colourTarget, 0);
    // 9-tap box kernel
    vec4 colour = texture(colourTarget, texCoord);
    colour += texture(colourTarget, texCoord + texel * vec2(-1,1));
    colour += texture(colourTarget, texCoord + texel * vec2(-1,0));
    colour += texture(colourTarget, texCoord + texel * vec2(-1,-1));
    colour += texture(colourTarget, texCoord + texel * vec2(0,1));
    colour += texture(colourTarget, texCoord + texel * vec2(0,-1));
    colour += texture(colourTarget, texCoord + texel * vec2(1,1));
    colour += texture(colourTarget, texCoord + texel * vec2(1,0));
    colour += texture(colourTarget, texCoord + texel * vec2(1,-1));
    return colour / 9;
}

vec4 Distort(vec2 texCoord)
{
    vec2 mid = vec2(0.5f);
    float distanceFromCentre = distance(texCoord, mid);
    vec2 normalizedCoord = normalize(texCoord - mid);
    float bias = distanceFromCentre +
    sin(distanceFromCentre * 15) * 0.05f;
    vec2 newCoord = mid + bias * normalizedCoord;
    return texture(colourTarget, newCoord);
}

vec4 ScanLine(vec2 texCoord)
{
    vec4 colour = texture(colourTarget, texCoord);

    // Num of ScanLines
    float Density = 10;
    float Count = textureSize(colourTarget, 0).y * Density;
 
    //float ScanLineMultiplier = sin(int(texCoord.y) % Count);


    //colour *= sin(texCoord.y * ProgressPercent);
    //colour *= ProgressPercen
    //colour *= ScanLineMultiplier;
    return colour + colour * vec4(sin(texCoord.y * Count - ProgressPercent + 1),sin(texCoord.y * Count - ProgressPercent),sin(texCoord.y * Count - ProgressPercent - 1),1);
}

vec4 DistanceFog(vec2 texCoord)
{
    vec4 colour = texture(colourTarget, texCoord);
    return vec4(vec3(gl_FragCoord.z), 1);
}

vec4 Toon(vec2 texCoord)
{
    vec4 colour = texture(colourTarget, texCoord);
    return round(colour * float(ToonScale)) / ToonScale;
}

vec4 Gray(vec2 texCoord)
{
    vec4 colour = texture(colourTarget, texCoord);
    float Average = (colour.x + colour.y + colour.z) / 3.0f;
    return vec4(Average,Average,Average,1);
}

vec4 ColorCheck(vec2 texCoord)
{
    vec2 texel = 1.0f / textureSize(colourTarget, 0);
    vec4 colour = texture(colourTarget, texCoord);
    colour += vec4(texCoord, 0, 1);
    return colour;
}

void main()
{
    // calculate texel size
    vec2 texSize = textureSize( colourTarget, 0 );
    vec2 texelSize = 1.0f / texSize;
    // adjust texture coordinate
    vec2 scale = (texSize - texelSize) / texSize;
    vec2 texCoord = vTexCoord / scale + texelSize * 0.5f;

    // sample post effect
    switch(PostProcess)
    {
        case 0:
            FragColour = Default(texCoord);
            break;
        case 1:
            FragColour = BoxBlur(texCoord);
            break; 
        case 2:
            FragColour = Distort(texCoord);
            break;
        case 3:
            FragColour = ScanLine(texCoord);
            break;
        case 4:
            FragColour = Toon(texCoord);
            break;
        case 5:
            FragColour = Gray(texCoord);
            break;
        case 6:
            FragColour = DistanceFog(texCoord);
            break;
        case 7:
            FragColour = ColorCheck(texCoord);
            break;
    }

    //FragColour = BoxBlur(texCoord);
    //FragColour = vec4(1,0,1,1);
}

