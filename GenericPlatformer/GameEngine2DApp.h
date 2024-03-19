#pragma once

#include "Application.h"
#include "Renderer2D.h"

class Transform;
class APlayer;

// Managers

class GameEngine2DApp : public aie::Application
{
public:
	GameEngine2DApp();
	virtual ~GameEngine2DApp();

	virtual bool startup() override;
	virtual void shutdown() override;
	virtual void update(float deltaTime) override;
	virtual void draw() override;

public:
	aie::Font* m_font;

	bool m_Debug = true;
	bool m_IsPaused = false;
};