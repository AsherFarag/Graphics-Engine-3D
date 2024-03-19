#pragma once
#include "Manager.h"

#include <queue>

class PhysicsObject;
class RigidBody;

#define DEFAULT_GRAVITY vec2(0, -9.82f)

class PhysicsManager :
    public Manager
{
public:
    PhysicsManager();
    ~PhysicsManager() {}

    virtual void Init() override;
    virtual void Update(float DeltaTime) override;
    virtual void Draw() override;

public:
    vec2 GetGravity() const { return m_Gravity; }
    void SetGravity(vec2 a_Gravity) { m_Gravity = a_Gravity; }

    float GetTimeStep() const { return m_TimeStep; }
    void SetTimeStep(float a_TimeStep) { m_TimeStep = a_TimeStep; }

    float GetTotalEnergy();

    void AddPhysicsObject(PhysicsObject* a_Component);
    void RemovePhysicsObject(PhysicsObject* a_Component);

    void AddRigidBody(RigidBody* a_Component);
    void RemoveRigidBody(RigidBody* a_Component);

    void ApplyContactForces(RigidBody* a_RB1, RigidBody* a_RB2, vec2 a_Normal, float Penetration);

protected:
    vec2 m_Gravity = DEFAULT_GRAVITY;
    float m_TimeStep = 0.02f;

public:
    std::vector<PhysicsObject*> m_PhysicsObjects;
    std::vector<RigidBody*> m_RigidBodies;

//    // Debug
//    typedef void(Component::*DebugDrawCall)();
//    std::vector<DebugDrawCall*> m_DebugDrawCalls;
//public:
//    void AddDebugDrawCall(DebugDrawCall* a_DebugDrawCall) { m_DebugDrawCalls.push_back(a_DebugDrawCall); }
//    void RemoveDebugDrawCall(DebugDrawCall* a_DebugDrawCall) { m_DebugDrawCalls.erase(std::find(m_DebugDrawCalls.begin(), m_DebugDrawCalls.end(), a_DebugDrawCall)); }

#pragma region Collision Scenarios

public:
    static bool PlaneToPlane(RigidBody*, RigidBody*);
    static bool PlaneToCircle(RigidBody*, RigidBody*);
    static bool PlaneToBox(RigidBody*, RigidBody*);
    static bool CircleToPlane(RigidBody*, RigidBody*);
    static bool CircleToCircle(RigidBody*, RigidBody*);
    static bool CircleToBox(RigidBody*, RigidBody*);
    static bool BoxToPlane(RigidBody*, RigidBody*);
    static bool BoxToCircle(RigidBody*, RigidBody*);
    static bool BoxToBox(RigidBody*, RigidBody*);

#pragma endregion
};

// QuadTree
//struct Node
//{
//    friend class PhysicsManager;
//
//    static std::queue<Node*> s_NodePool;
//    static Node* s_Root;
//    static const unsigned int MaxLevel = 10;
//
//    int x, y;
//    int w, h;
//    int m_Level; // How many levels below the root this node is
//
//    Node()
//    {
//    }
//
//    std::vector<Node*> m_Children;
//    std::vector<RigidBody*> m_RigidBodies;
//
//    Node* GetRoot() { return s_NodePool.front(); }
//
//    void ConstructQuadTree(PhysicsManager* a_PhysicsManager, vec2 a_WindowSize)
//    {
//        if (a_PhysicsManager == nullptr)
//            return;
//
//        if (s_Root == nullptr)
//            s_Root = new Node();
//        else
//            s_Root->Set(0, 0, a_WindowSize.x, a_WindowSize.y, 0);
//
//        for (auto RB : a_PhysicsManager->m_RigidBodies)
//            s_Root->AddRigidBody(RB);
//    }
//
//    void AddRigidBody(RigidBody* a_RB)
//    {
//        if (m_RigidBodies.size() < 4 || m_Level == MaxLevel)
//        {
//            m_RigidBodies.push_back(a_RB);
//        }
//        else
//        {
//            if (m_Children.size() == 0)
//            {
//                Node* Node1 = s_NodePool.front();
//                s_NodePool.pop();
//                Node* Node2 = s_NodePool.front();
//                s_NodePool.pop();
//                Node* Node3 = s_NodePool.front();
//                s_NodePool.pop();
//                Node* Node4 = s_NodePool.front();
//                s_NodePool.pop();
//
//                Node1->Set(x, y, w / 2, h / 2, m_Level + 1);
//                m_Children.push_back(Node1);
//                Node2->Set(x + w / 2, y, w / 2, h / 2, m_Level + 1);
//                m_Children.push_back(Node2);
//                Node3->Set(x, y + h / 2, w / 2, h / 2, m_Level + 1);
//                m_Children.push_back(Node3);
//                Node4->Set(x + w / 2, y + h / 2, w / 2, h / 2, m_Level + 1);
//                m_Children.push_back(Node4);
//
//
//                for (auto rb : m_RigidBodies)
//                {
//                    for (auto Child : m_Children)
//                    {
//                        if (Child->Inside(*rb))
//                        {
//                            Child->AddRigidBody(rb);
//                        }
//                    }
//                }
//            }
//
//            for (auto Child : m_Children)
//            {
//                if (Child->Inside(*a_RB))
//                {
//                    Child->AddRigidBody(a_RB);
//                }
//            }
//
//        }
//    }
//    void Draw()
//    {
//        aie::Gizmos::add2DLine(vec2(x, y), vec2(x, h), vec4(1));
//        aie::Gizmos::add2DLine(vec2(x, h), vec2(w, h), vec4(1));
//        aie::Gizmos::add2DLine(vec2(w, h), vec2(w, y), vec4(1));
//        aie::Gizmos::add2DLine(vec2(w, y), vec2(x, y), vec4(1));
//
//        for (auto Child : m_Children)
//            Child->Draw();
//    }
//
//private:
//    bool Inside(RigidBody& a_RB)
//    {
//        vec2 RBPosition = a_RB.GetPosition();
//        vec2 RBSize = a_RB.GetBoundingBoxSize();
//
//        vec2 Point1 = RBPosition - RBSize;
//        vec2 Point2 = RBPosition + RBSize;
//
//        // Simple AABB Check
//        if ((Point1.x >= x && Point1.x <= (x + w)) || (Point2.x >= x && Point2.x <= (x + w)) &&
//            (Point1.y >= y && Point1.x <= (y + h)) || (Point1.y >= y && Point1.x <= (y + h)))
//            return true;
//        else
//            return false;
//    }
//
//    void Set(int a_x, int a_y, int a_w, int a_h, int a_Level)
//    {
//        for (auto Child : m_Children)
//            s_NodePool.push(Child);
//
//        m_Children.clear();
//        m_RigidBodies.clear();
//        x = a_x;
//        y = a_y;
//        w = a_w;
//        h = a_h;
//        m_Level = a_Level;
//    }
//};
