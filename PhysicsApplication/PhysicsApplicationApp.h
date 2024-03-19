#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "Gizmos.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include <string>
#include "PhysicsScene.h"
#include "RocketShip.h"

using namespace glm;

class PhysicsApplicationApp : public aie::Application {
public:

	PhysicsApplicationApp();
	virtual ~PhysicsApplicationApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:
	aie::Renderer2D*	m_2DRenderer;
	aie::Font*			m_Font;
	PhysicsScene* m_PhysicsScene;

};