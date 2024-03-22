#include "GraphicsEngine3DApp.h"

int main()
{
	// initialise and loop
	GraphicsEngine3DApp::GetInstance()->run("Real Engine", 1920 * 0.5f, 1080 * 0.5f, false);

	return 0;
}