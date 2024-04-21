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
    const auto& GetOwner() const { return m_Owner; }
    void SetOwner(AActor* a_Owner);

#pragma region ImGui

public:
    virtual void Draw_ImGui() {};

#pragma endregion

};

