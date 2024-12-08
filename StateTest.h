#pragma once

#include <4dm.h>

using namespace fdm;

class StateTest : public State
{
public:
	static StateTest instanceObj;

	QuadRenderer qr;
	const Shader* qs;

	FontRenderer font;

	// ui
	gui::Interface ui{};
	gui::Button btn1{};
	gui::Text text{};

	int clicks = 0;

	StateTest() {}

	void updateProjection(const glm::ivec2& size);
	static void viewportCallback(void* user, const glm::ivec4& pos, const glm::ivec2& scroll);

	void init(StateManager& s) override;
	void close(StateManager& s) override;
	void render(StateManager& s) override;
	void mouseInput(StateManager& s, double xpos, double ypos) override;
	void scrollInput(StateManager& s, double xoff, double yoff) override;
	void mouseButtonInput(StateManager& s, int button, int action, int mods) override;
	void keyInput(StateManager&, int key, int scancode, int action, int mods) override;
	void windowResize(StateManager&, int width, int height) override;
	void charInput(StateManager&, uint32_t codepoint) override;
	
	static void btn1Callback(void* user);
};