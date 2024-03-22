#pragma once
#include "UBaseObject.h"

// --- Engine ---
class AActor;

class UBaseComponent :
    public UBaseObject
{
public:
    UBaseComponent() {}
    UBaseComponent(AActor* a_Owner);
    virtual ~UBaseComponent();

protected:
    AActor* m_Owner = nullptr;

    virtual void OnEnabled() {};
    virtual void OnDisabled() {};

    virtual void OnDestroyed() {};

public:
    void SetOwner(AActor* a_Owner);

#pragma region ImGui

public:
    virtual void Draw_ImGui() {};

#pragma endregion

};
