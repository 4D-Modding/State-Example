#define DEBUG_CONSOLE // Uncomment this if you want a debug console

// Mod Name. Make sure it matches the mod folder's name
#define MOD_NAME "Custom-State"
#define MOD_VER "1.0"

#include <4dm.h>

#include "TestState.h"

#include <Windows.h>
#include <cstdio>

#include <glm/gtx/string_cast.hpp>

using namespace fdm;

TestState* testStateL = nullptr;

void(__thiscall* GameState_keyInput)(GameState* self, StateManager* s, int key, int scancode, int action, int mods);
void __fastcall GameState_keyInput_H(GameState* self, StateManager* s, int key, int scancode, int action, int mods)
{
	if (testStateL == nullptr)
		testStateL = new TestState();

	// open TestState when pressing J
	if (key == GLFW_KEY_J && action == GLFW_PRESS)
	{
		testStateL->init(*s);

		self->pause(*s);

		s->states.push_back(testStateL);
	}

	return GameState_keyInput(self, s, key, scancode, action, mods);
}
DWORD WINAPI Main_Thread(void* hModule)
{
	// Create console window if DEBUG_CONSOLE is defined
#ifdef DEBUG_CONSOLE
	AllocConsole();
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);
#endif

	glewInit();
	glfwInit();

	// Hook to the GameState::keyInput function
	Hook(reinterpret_cast<void*>(base + idaOffsetFix(0x53880)), reinterpret_cast<void*>(&GameState_keyInput_H), reinterpret_cast<void**>(&GameState_keyInput));

	EnableHook(0);

	return true;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD _reason, LPVOID lpReserved)
{
	if (_reason == DLL_PROCESS_ATTACH)
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Main_Thread, hModule, 0, NULL);
	return TRUE;
}
