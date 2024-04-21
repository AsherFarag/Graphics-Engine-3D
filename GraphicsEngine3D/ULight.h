#pragma once
#include "UBaseComponent.h"

/*
*   Simple Base Light Class that works as an ambient light.
*/

class ULight :
    public UBaseComponent
{
public:
    ULight() {}
    ULight( vec4 a_Colour, float a_Intensity = 1.f )
        : m_Colour( a_Colour ), m_Intensity( a_Intensity ) {}

    /* Returns: Colour multipled by the Intensity */
    vec4 GetTotalColour() { return m_Colour * m_Intensity; }

    vec4 GetColour() { return m_Colour; }
    float GetIntensity() { return m_Intensity; }

    void SetColour( vec4 a_Colour ) { m_Colour = a_Colour; }
    void SetIntensity( float a_Intensity ) { m_Intensity = a_Intensity; }

protected:
    vec4 m_Colour     = vec4( 1 );
    float m_Intensity = 1.f;

public:
    virtual void Draw_ImGui() {}
};

