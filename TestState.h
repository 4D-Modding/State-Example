#pragma once

#include <4dm.h>

using namespace fdm;

class TestState : public State
{
public:
	static TestState* instanceObj;

	QuadRenderer qr{};
	const Shader* qs;
	FontRenderer font;

	// ui
	gui::Interface ui{};
	gui::Button btn1{};
	gui::Text text{};

	int clicks = 0;

	void updateShaders(const glm::mat4& proj);
	void updateProjection(const glm::ivec2& size);
	// thanks mashpoe for giving me this very cool thingy which you need to make some UI things work
	static void viewportCallbackFunc(void* user, const glm::ivec4& pos, const glm::ivec2& scroll);
	static void btn1Callback(void* user);
	void init(StateManager& s) override;
	void render(StateManager& s) override;
	void mouseInput(StateManager& s, double xPos, double yPos) override;
	void scrollInput(StateManager& s, double xOff, double yOff) override;
	void mouseButtonInput(StateManager& s, int btn, int action, int mods) override;
	void keyInput(StateManager& s, int key, int scancode, int action, int mods) override;
	void close(StateManager& s) override;
	void windowResize(StateManager& s, int w, int h) override;
	void update(StateManager& s, double deltaTime) override;
};