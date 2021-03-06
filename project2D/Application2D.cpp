#include "Application2D.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "Vector3.h"
#include "SceneObject.h"
#include "SpriteObject.h"
#include "Timer.h"


Application2D::Application2D() {

}

Application2D::~Application2D() {

}


bool Application2D::startup() {

	m_start.reset();
	m_2dRenderer = new aie::Renderer2D();

	// Load textures
	//m_texture = new aie::Texture("./textures/numbered_grid.tga");
	//m_shipTexture = new aie::Texture("./textures/ship.png");

	m_tank.load("./textures/Tank.png");
	m_turret.load("./textures/GunTurret.png");

	// Attach turret to tank
	m_tank.addChild(&m_turret);

	// Center the tank
	m_tank.setPosition(getWindowWidth() / 2.f, getWindowHeight() / 2.f);

	m_font = new aie::Font("./font/consolas.ttf", 32);
	
	m_cameraX = 0;
	m_cameraY = 0;
	m_timer = 0;

	return true;
}

void Application2D::shutdown() {
	
	delete m_font;
	//delete m_tank;
	//delete m_turret;
	delete m_2dRenderer;
}

void Application2D::update(float deltaTime) {
	
	//Timer updateTimer;

	m_timer += deltaTime;
	m_tank.update(deltaTime);

	// input example
	aie::Input* input = aie::Input::getInstance();

	float tankSpin = 3;
	float turretSpin = 5;

	// Rotate the tank
	if (input->isKeyDown(aie::INPUT_KEY_A))
		m_tank.rotate(deltaTime*tankSpin);
	if (input->isKeyDown(aie::INPUT_KEY_D))
		m_tank.rotate(-deltaTime*tankSpin);

	// Move tank, the 100 magic-number represents speed
	if (input->isKeyDown(aie::INPUT_KEY_W)) {
		auto facing = m_tank.getLocalTransform()[1] * 200;
		m_tank.accelerate(facing.x, facing.y);
	}
	if (input->isKeyDown(aie::INPUT_KEY_S)) {
		auto facing = m_tank.getLocalTransform()[1] * -200;
		m_tank.accelerate(facing.x, facing.y);
	}

	// Rotate turret
	if (input->isKeyDown(aie::INPUT_KEY_LEFT))
		m_turret.rotate(deltaTime*turretSpin);
	if (input->isKeyDown(aie::INPUT_KEY_RIGHT))
		m_turret.rotate(-deltaTime*turretSpin);

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();

	//m_TimeSpentInUpdate += updateTimer.nanoseconds();
	//m_numUpdates++;
}

void Application2D::draw() {

	float elapsedSeconds = m_start.seconds(); //todo use timer
	

	// wipe the screen to the background colour
	clearScreen();

	// set the camera position before we begin rendering
	m_2dRenderer->setCameraPos(m_cameraX, m_cameraY);

	// begin drawing sprites
	m_2dRenderer->begin();

	// Draw the Tank
	m_tank.draw(m_2dRenderer);

	//// demonstrate animation
	//m_2dRenderer->setUVRect(int(m_timer) % 8 / 8.0f, 0, 1.f / 8, 1.f / 8);
	//m_2dRenderer->drawSprite(m_texture, 200, 200, 100, 100);
	//
	//// demonstrate spinning sprite
	//m_2dRenderer->setUVRect(0,0,1,1);
	//m_2dRenderer->drawSprite(m_shipTexture, 600, 400, 0, 0, m_timer, 1);
	//
	//// draw a thin line
	//m_2dRenderer->drawLine(300, 300, 600, 400, 2, 1);
	//
	//// draw a moving purple circle
	//m_2dRenderer->setRenderColour(1, 0, 1, 1);
	//m_2dRenderer->drawCircle(sin(m_timer) * 100 + 600, 150, 50);
	//
	//// draw a rotating red box
	//m_2dRenderer->setRenderColour(1, 0, 0, 1);
	//m_2dRenderer->drawBox(600, 500, 60, 20, m_timer);
	//
	//// draw a slightly rotated sprite with no texture, coloured yellow
	//m_2dRenderer->setRenderColour(1, 1, 0, 1);
	//m_2dRenderer->drawSprite(nullptr, 400, 400, 50, 50, 3.14159f * 0.25f, 1);
	//
	//
	// output some text, uses the last used colour
	//float elapsedSeconds = m_timer;

	
	char fps[32];
	sprintf_s(fps, 32, "FPS: %i", getFPS());
	m_2dRenderer->drawText(m_font, fps, 0, 720 - 32);
	m_2dRenderer->drawText(m_font, "Press ESC to quit!", 0, 720 - 64);

	char elapsed[32];
	sprintf_s(elapsed, 32, "Time elapsed: %8.2f", elapsedSeconds);
	m_2dRenderer->drawText(m_font, elapsed , 0, 720 - 128);

	// done drawing sprites
	m_2dRenderer->end();
}