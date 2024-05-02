#pragma once
#include "UBaseComponent.h"

class UTransform :
    public UBaseComponent
{
    INSPECTABLE;
public:
    UTransform() {}
    UTransform(vec3 a_Position);
    UTransform(quat a_Rotation);
    UTransform(float a_Scale);
    UTransform(vec3 a_Position, quat a_Rotation);
    UTransform(vec3 a_Position, quat a_Rotation, vec3 a_Scale);
    UTransform(vec3 a_Position, quat a_Rotation, float a_Scale);

    const mat4& GetLocalTransformation() { return m_Transform; }
    const vec3& GetLocalPosition()       { return m_Position; }
    const quat& GetLocalRotation()       { return m_Rotation; }
    const vec3& GetLocalRotationEular()  { return glm::eulerAngles(m_Rotation); }
    const vec3& GetLocalScale()          { return m_Scale; }

    const mat4& GetTransformation();
    const vec3& GetPosition();
    const quat& GetRotation();
    const vec3& GetRotationEular();
    const vec3& GetScale();
    vec3& GetForward();

    void SetTransform(const mat4& a_Transform);
    void SetPosition (const vec3& a_Position);
    void SetRotation (const quat& a_Rotation);
    void SetRotation (const vec3& a_Rotation);
    void SetScale    (const vec3& a_Scale);

    bool IsDirty() { return m_Dirty; }
    void Update();

    void SetParentTransform( UTransform* a_NewParent ) { m_Parent = a_NewParent; }

private:
    UTransform* m_Parent = nullptr;
    mat4 m_Transform = mat4(1);
    vec3 m_Position  = vec3();
    quat m_Rotation  = quat();
    vec3 m_Scale     = vec3(1);

    bool m_Dirty = false; // If true, the transform will be updated and then it will no longer be dirty.

#if IS_EDITOR
public:
    virtual void Draw_ImGui();
#endif // IS_EDITOR

};

