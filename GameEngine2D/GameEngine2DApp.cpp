#include "GameEngine2DApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "World.h"
#include "Gizmos.h"
#include "glm/ext.hpp"

#include "iostream"

#include "Transform.h"
#include "Renderer.h"

#include "BoxRB.h"
#include "CircleRB.h"
#include "PlaneRB.h"

#include "ASoftBody.h"

GameEngine2DApp::GameEngine2DApp()
{

}

GameEngine2DApp::~GameEngine2DApp()
{

}

bool GameEngine2DApp::startup()
{
	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);

	aie::Gizmos::create(255U, 255U, 65535U, 65535U);

	vec2 WindowSize = vec2(getWindowWidth(), getWindowHeight());
	World::TestScene(WindowSize);

	World::GetPhysicsManager()->SetGravity(vec2(0, 5.f * -9.82f));

	return true;
}

void GameEngine2DApp::shutdown()
{
	delete m_font;
}

void GameEngine2DApp::update(float DeltaTime)
{
	aie::Input* input = aie::Input::getInstance();

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();

	// Pause
	if (input->wasKeyReleased(aie::INPUT_KEY_P))
		m_IsPaused = !m_IsPaused;

	if (m_IsPaused == false)
		World::GetPhysicsManager()->Update(DeltaTime);
}

void GameEngine2DApp::draw()
{
	// wipe the screen to the background colour
	clearScreen();
	aie::Gizmos::clear();

	// begin drawing sprites
	World::GetRenderer2D()->begin();



	World::GetRenderer()->Draw();

	// Debug Draw
	if (m_Debug)
	{
		World::GetPhysicsManager()->Draw();
		aie::Gizmos::draw2D(ortho<float>(0, getWindowWidth(), 0, getWindowHeight(), -1.f, 1.f));
	}



	// output some text, uses the last used colour
	if (m_font->getTextureHandle() != 0)
	{
		char FPSText[32];
		sprintf_s(FPSText, 32, "FPS: %i", getFPS());
		World::GetRenderer2D()->drawText(m_font, FPSText, 0, 0);

		World::GetRenderer2D()->drawText(m_font, std::to_string(World::GetPhysicsManager()->GetTotalEnergy()).c_str(), 0, getWindowHeight() - 30);
		//World::GetRenderer2D()->drawText(m_font, "Press ESC to quit", 0, 0);
	}

	// done drawing sprites
	World::GetRenderer2D()->end();


}