#pragma once
#include "UBaseComponent.h"

// --- GLM ---
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

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

    auto& GetTransformation()     { return m_Transform; }
    auto& GetPosition()      { return m_Position; }
    auto& GetRotation()      { return m_Rotation; }
    auto& GetRotationEular() { return glm::eulerAngles(m_Rotation); }
    auto& GetScale()         { return m_Scale; }
    vec3& GetForward();

    void SetTransform(const mat4& a_Transform);
    void SetPosition (const vec3& a_Position);
    void SetRotation (const quat& a_Rotation);
    void SetRotation (const vec3& a_Rotation);
    void SetScale    (const vec3& a_Scale);

    bool IsDirty() { return m_Dirty; }
    void Update();

private:
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

