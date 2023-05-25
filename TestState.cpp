#include "TestState.h"

TestState* TestState::instanceObj = nullptr;

void TestState::updateShaders(const glm::mat4& proj)
{
	const Shader* textShader = ShaderManager::get("textShader");
	textShader->use();
	glUniformMatrix4fv(glGetUniformLocation(textShader->ID, "P"), 1, GL_FALSE, &proj[0][0]);

	const Shader* tex2DShader = ShaderManager::get("tex2DShader");
	tex2DShader->use();
	glUniformMatrix4fv(glGetUniformLocation(tex2DShader->ID, "P"), 1, GL_FALSE, &proj[0][0]);

	const Shader* quadShader = ShaderManager::get("quadShader");
	quadShader->use();
	glUniformMatrix4fv(glGetUniformLocation(quadShader->ID, "P"), 1, GL_FALSE, &proj[0][0]);
}

void TestState::updateProjection(const glm::ivec2& size)
{
	glm::mat4 projection2D = glm::ortho(0.f, (float)size.x, (float)size.y, 0.f, -1.0f, 1.0f);
	glViewport(0, 0, size.x, size.y);
	updateShaders(projection2D);
}
// thanks mashpoe for giving me this very cool thingy which you need to make some UI things work
void TestState::viewportCallbackFunc(void* user, const glm::ivec4& pos, const glm::ivec2& scroll)
{
	GLFWwindow* window = (GLFWwindow*)user;

	// update the render viewport

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

void TestState::btn1Callback(void* user)
{
	TestState::instanceObj->clicks++;
	
	TestState::instanceObj->text.setText(std::to_string(TestState::instanceObj->clicks));
}

void TestState::init(StateManager& s)
{
	instanceObj = this;

	int width, height;
	glfwGetWindowSize(s.window, &width, &height);

	qs = ShaderManager::get("quadShader");

	updateProjection({ width, height });

	font = { ResourceManager::get("pixelfont.png"), ShaderManager::get("textShader") };

	qr.shader = qs;
	qr.init();

	ui.window = s.window;
	ui.font = &font;
	ui.qr = &qr;
	ui.viewportCallback = viewportCallbackFunc;
	ui.viewportUser = s.window;

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
}

void TestState::render(StateManager& s)
{
	// clear color and depth buffers stuff ig
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// render gamestate
	glDepthMask(1);
	s.states[s.states.size() - 2]->render(s);

	// get width and height of window
	int w, h;
	glfwGetWindowSize(s.window, &w, &h);

	glDepthMask(0); // turn off depth mask

	// render bg with color r:0.1 g:0.1 b:0.1 a:0.6
	qr.setColor(0.1f, 0.1f, 0.1f, 0.6f);
	qr.setPos(0, 0, w, h);
	if (qr.mode != 4)
	{
		if (qr.VAO)
		{
			int data[5] = { 0, 1, 2, 2, 3 };
			glBindVertexArray(qr.VAO);
			glBindBuffer(GL_ARRAY_BUFFER, qr.buffers[0]);
			glBufferData(GL_ARRAY_BUFFER, 24, data, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, NULL);
		}
		qr.mode = 4;
		qr.elementCount = 6;
	}
	glUseProgram(qr.shader->id());
	glBindVertexArray(qr.VAO);
	glDrawElements(qr.mode, qr.elementCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(NULL);
	
	// render ui
	ui.render();
}
void TestState::mouseInput(StateManager& s, double xPos, double yPos)
{
	ui.mouseInput(xPos, yPos);
}
void TestState::scrollInput(StateManager& s, double xOff, double yOff)
{
	ui.scrollInput(xOff, yOff);
}
void TestState::keyInput(StateManager& s, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) // close state
	{
		s.states[s.states.size() - 2]->resume(s); // resume gamestate
		close(s);
		s.states.erase(std::remove(s.states.begin(), s.states.end(), this), s.states.end()); // remove self from states list
	}
	else
		ui.keyInput(key, scancode, action, mods);
}
void TestState::mouseButtonInput(StateManager& s, int btn, int action, int mods)
{
	ui.mouseButtonInput(btn, action, mods);
}
void TestState::close(StateManager& s)
{
	ui.clear();
}
void TestState::windowResize(StateManager& s, int w, int h)
{
	updateProjection({ w, h });
}
void TestState::update(StateManager& s, double deltaTime)
{

}