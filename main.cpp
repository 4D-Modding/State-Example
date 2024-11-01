//#define DEBUG_CONSOLE // Uncomment this if you want a debug console to start. You can use the Console class to print. You can use Console::inStrings to get input.

#include <4dm.h>

using namespace fdm;

#include "StateTest.h"

// Initialize the DLLMain
initDLL

$hook(void, StateGame, keyInput, StateManager& s, int key, int scancode, int action, int mods)
{
	// open StateTest by pressing J
	if (key == GLFW_KEY_J && action == GLFW_PRESS)
	{
		s.pushState(&StateTest::instanceObj);
	}

	original(self, s, key, scancode, action, mods);
}

$hook(void, StateIntro, init, StateManager& s)
{
	original(self, s);

	// initialize opengl stuff
	glewExperimental = true;
	glewInit();
	glfwInit();
}
