#include "PhysicsApplicationApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "Plane.h"


PhysicsApplicationApp::PhysicsApplicationApp()
{

}

PhysicsApplicationApp::~PhysicsApplicationApp()
{

}

bool PhysicsApplicationApp::startup()
{
	aie::Gizmos::create(255U, 255U, 65535U, 65535U);

	m_2DRenderer = new aie::Renderer2D();

	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_Font = new aie::Font("../bin/font/consolas.ttf", 32);

	// Implement PhysicsScene ==========
	m_PhysicsScene = new PhysicsScene();
	m_PhysicsScene->SetTimeStep(0.01f);
	//==================================

	//RocketShip* m_RocketShip = new RocketShip(glm::vec2(getWindowWidth() / 2, getWindowHeight() / 2), 1.f, 5.f, 15);
	//m_PhysicsScene->AddActor(m_RocketShip);

	m_PhysicsScene->SetGravity(vec2(0, 3.f * -9.82f));
	//m_PhysicsScene->SetGravity(vec2(0));

	Sphere* ball1 = new Sphere(vec2(15.5f, 25), vec2(0), 4.0f, 3, vec4(1, 0, 0, 1), 0.9f);
	//Sphere* ball2 = new Sphere(vec2(10, 0), vec2(0), 4.0f, 3, vec4(0, 1, 0, 1), 1.f);
	Plane* wall1 = new Plane(vec2(1, 0), -70);
	Plane* wall2 = new Plane(vec2(-1, 0), -70);
	Plane* plane2 = new Plane(vec2(0, 1), -10);

	m_PhysicsScene->AddActor(ball1);
	//m_PhysicsScene->AddActor(ball2);
	m_PhysicsScene->AddActor(wall1);
	m_PhysicsScene->AddActor(wall2);
	m_PhysicsScene->AddActor(plane2);

	return true;
}

void PhysicsApplicationApp::shutdown()
{
	delete m_Font;
	delete m_2DRenderer;
	delete m_PhysicsScene;
}

void PhysicsApplicationApp::update(float deltaTime)
{
	aie::Input* input = aie::Input::getInstance();
	aie::Gizmos::clear();
	// Implement PhysicsScene ==========
	m_PhysicsScene->Update(deltaTime);
	//==================================

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void PhysicsApplicationApp::draw()
{

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2DRenderer->begin();

	m_PhysicsScene->Draw();

	const float AspectRatio = (float)getWindowWidth() / (float)getWindowHeight();
	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100, -100 / AspectRatio, 100 / AspectRatio, -1.f, 1.f));

	// output some text, uses the last used colour
	char FPSText[32];
	sprintf_s(FPSText, 32, "FPS: %i", getFPS());
	m_2DRenderer->drawText(m_Font, FPSText, 0, getWindowHeight() - m_Font->getStringHeight(FPSText));

	// done drawing sprites
	m_2DRenderer->end();
}