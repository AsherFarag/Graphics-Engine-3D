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
#include "APlayer.h"

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
	m_font = new aie::Font("./font/consolas.ttf", 32);

	aie::Gizmos::create(255U, 255U, 65535U, 65535U);
	World::GetPhysicsManager()->SetGravity(vec2(0, 10.f * -9.82f));
	vec2 WindowSize = vec2(getWindowWidth(), getWindowHeight());
	World::SetApplication(this);
	World::SetWindowSize(WindowSize);
	World::TestScene(WindowSize);
	//World::TestBoxScene(WindowSize, 10);
	World::GetPhysicsManager()->SetGravity(2.f * DEFAULT_GRAVITY);
	World::GetWorld()->ReachedGoal();
	
	//World::TestBallScene(WindowSize, 5);
	//World::TestBoxScene(WindowSize, 5);

	//std::vector<std::string> sb;
	//sb.push_back("###############################");
	//sb.push_back("#.............................#");
	//sb.push_back("#.............................#");
	//sb.push_back("#.............................#");
	//sb.push_back("#.............................#");
	//sb.push_back("#.............................#");
	//sb.push_back("#.............................#");
	//sb.push_back("#.............................#");
	//sb.push_back("#.............................#");
	//sb.push_back("#.............................#");
	//sb.push_back("#.............................#");
	//sb.push_back("#.............................#");
	//sb.push_back("#.............................#");
	//sb.push_back("#.............................#");
	//sb.push_back("#.............................#");
	//sb.push_back("#.............................#");
	//sb.push_back("#.............................#");
	//sb.push_back("###############################");

	//World::BuildLevel(WindowSize, sb);


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
		World::Update(DeltaTime);
}

void GameEngine2DApp::draw()
{
	// wipe the screen to the background colour
	clearScreen();
	aie::Gizmos::clear();

	// begin drawing sprites
	World::GetRenderer2D()->begin();

	// Debug Draw
	if (m_Debug)
	{
		World::GetPhysicsManager()->Draw();
	}

	World::Draw();

	aie::Gizmos::draw2D(ortho<float>(0, getWindowWidth(), 0, getWindowHeight(), -1.f, 1.f));

	// output some text, uses the last used colour
	if (m_font->getTextureHandle() != 0)
	{
		//char TimesShotText[32];
		//sprintf_s(TimesShotText, 32, "Times Shot: %i", m_Player->GetTimesShot());
		//World::GetRenderer2D()->drawText(m_font, TimesShotText, 0, getWindowHeight() - m_font->getStringHeight(TimesShotText));

		char FPSText[32];
		sprintf_s(FPSText, 32, "FPS: %i", getFPS());
		World::GetRenderer2D()->drawText(m_font, FPSText, 0, 0);
	}

	// done drawing sprites
	World::GetRenderer2D()->end();


}