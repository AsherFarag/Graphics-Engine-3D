#pragma once
#include "ImGui_Window.h"
#include "Inspector.hpp"

#include <typeinfo>

class ImGui_Inspector :
    public ImGui_Window
{
public:
    ImGui_Inspector();

    void* InspectedActor;

private:
    Inspector m_Inspector;

protected:
    virtual void BeginDraw();
    virtual void OnDraw();
    virtual void EndDraw();
};

