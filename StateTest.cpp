#include "StateTest.h"

StateTest StateTest::instanceObj{};

void StateTest::updateProjection(const glm::ivec2& size)
{
	glm::mat4 projection2D = glm::ortho(0.f, (float)size.x, (float)size.y, 0.f, -1.0f, 1.0f);
	glViewport(0, 0, size.x, size.y);

	// update all 2D shaders
	const Shader* textShader = ShaderManager::get("textShader");
	textShader->use();
	glUniformMatrix4fv(glGetUniformLocation(textShader->ID, "P"), 1, GL_FALSE, &projection2D[0][0]);

	const Shader* tex2DShader = ShaderManager::get("tex2DShader");
	tex2DShader->use();
	glUniformMatrix4fv(glGetUniformLocation(tex2DShader->ID, "P"), 1, GL_FALSE, &projection2D[0][0]);

	const Shader* quadShader = ShaderManager::get("quadShader");
	quadShader->use();
	glUniformMatrix4fv(glGetUniformLocation(quadShader->ID, "P"), 1, GL_FALSE, &projection2D[0][0]);
}

void StateTest::viewportCallback(void* user, const glm::ivec4& pos, const glm::ivec2& scroll)
{
	GLFWwindow* window = (GLFWwindow*)user;

	// update the viewport
	int wWidth, wHeight;
	glfwGetWindowSize(window, &wWidth, &wHeight);
	glViewport(pos.x, wHeight - pos.y - pos.w, pos.z, pos.w);

	// create a 2D projection matrix from the specified dimensions and scroll position
	glm::mat4 projection2D = glm::ortho(0.0f, (float)pos.z, (float)pos.w, 0.0f, -1.0f, 1.0f);
	projection2D = glm::translate(projection2D, { scroll.x, scroll.y, 0 });

	// update all 2D shaders
	const Shader* textShader = ShaderManager::get("textShader");
	textShader->use();
	glUniformMatrix4fv(glGetUniformLocation(textShader->ID, "P"), 1, GL_FALSE, &projection2D[0][0]);

	const Shader* tex2DShader = ShaderManager::get("tex2DShader");
	tex2DShader->use();
	glUniformMatrix4fv(glGetUniformLocation(tex2DShader->ID, "P"), 1, GL_FALSE, &projection2D[0][0]);

	const Shader* quadShader = ShaderManager::get("quadShader");
	quadShader->use();
	glUniformMatrix4fv(glGetUniformLocation(quadShader->ID, "P"), 1, GL_FALSE, &projection2D[0][0]);
}

void StateTest::btn1Callback(void* user)
{
	StateTest::instanceObj.clicks++;
	
	// update the text
	StateTest::instanceObj.text.setText(std::to_string(StateTest::instanceObj.clicks));
}

void StateTest::init(StateManager& s)
{
	int width, height;
	glfwGetWindowSize(s.window, &width, &height);

	qs = ShaderManager::get("quadShader");

	font = { ResourceManager::get("pixelfont.png"), ShaderManager::get("textShader") };

	qr.shader = qs;
	qr.init();

	ui = gui::Interface{ s.window };
	ui.viewportCallback = viewportCallback;
	ui.viewportUser = s.window;
	ui.font = &font;
	ui.qr = &qr;

	btn1.text = "+1";
	btn1.alignX(gui::ALIGN_CENTER_X);
	btn1.alignY(gui::ALIGN_CENTER_Y);
	btn1.width = 350;
	btn1.height = 50;
	btn1.callback = btn1Callback;
	btn1.user = s.window;

	text.setText(std::to_string(clicks));
	text.size = 4;
	text.shadow = true;
	text.fancy = true;
	text.alignX(gui::ALIGN_CENTER_X);
	text.offsetY(150);

	ui.addElement(&btn1);
	ui.addElement(&text);

	updateProjection({ width, height });
}

void StateTest::render(StateManager& s)
{
	// render the state behind
	glEnable(GL_DEPTH_TEST);
	s.states[s.states.size() - 2]->render(s);

	// get width and height of window
	int w, h;
	glfwGetWindowSize(s.window, &w, &h);

	glDisable(GL_DEPTH_TEST); // disable depth (since you are rendering 2D)

	// render a background using the QuadRenderer
	qr.setColor(0, 0, 0, 0.6);
	qr.setPos(0, 0, w, h);
	qr.setQuadRendererMode(GL_TRIANGLES);
	qr.render();
	
	// render the ui
	ui.render();
}
void StateTest::mouseInput(StateManager& s, double xPos, double yPos)
{
	ui.mouseInput(xPos, yPos);
}
void StateTest::scrollInput(StateManager& s, double xOff, double yOff)
{
	ui.scrollInput(xOff, yOff);
}
void StateTest::keyInput(StateManager& s, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) // close the state
		s.popState();
	else
		ui.keyInput(key, scancode, action, mods);
}
void StateTest::mouseButtonInput(StateManager& s, int btn, int action, int mods)
{
	ui.mouseButtonInput(btn, action, mods);
}
void StateTest::close(StateManager& s)
{
	ui.clear();
}
void StateTest::windowResize(StateManager& s, int w, int h)
{
	updateProjection({ w, h });
}
