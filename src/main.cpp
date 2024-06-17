#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <chrono>
#include <vector>

#include "Game/Game.h"
#include "Resources/ResourceManager.h"
#include "Renderer/Renderer.h"
#include "Physics/PhysicsEngine.h"

static constexpr unsigned int SCALE = 4;
static constexpr  unsigned int BLOCK_SIZE = 16;

glm::uvec2 g_windowSize(SCALE * 10 * BLOCK_SIZE, SCALE * 12 * BLOCK_SIZE);
std::unique_ptr<Game> g_game = std::make_unique<Game>(g_windowSize);

void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height)
{
    g_windowSize.x = width;
    g_windowSize.y = height;
	g_game->setWindowSize(g_windowSize);
}

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }

	g_game->setKey(key, action);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) 
{
	g_game->setButton(button, action);
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) 
{
	g_game->setMousePosition(xpos, ypos);
}

int main(int argc, char** argv)
{
	if (!glfwInit())
	{
		std::cout << "glfwInit failed" << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // ����� �������� ������ glfw
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* pWindow = glfwCreateWindow(g_windowSize.x, g_windowSize.y, "HMM", nullptr, nullptr);
	if (!pWindow)
	{
		std::cout << "glfwCreateWindow failed" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwSetWindowSizeCallback(pWindow, glfwWindowSizeCallback); // ������������ ����
	glfwSetKeyCallback(pWindow, glfwKeyCallback);

	glfwSetMouseButtonCallback(pWindow, mouse_button_callback);
	glfwSetCursorPosCallback(pWindow, cursor_position_callback);

	glfwMakeContextCurrent(pWindow); // ��������� �������� ����, ��� ��� ���� ����� ���� ���������

	if (!gladLoadGL()) // glad initialization
	{
		std::cout << "Can't load GLAD!" << std::endl;
	}

	std::cout << "Render: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

	RenderEngine::Renderer::setClearColor(0, 0, 0, 1);
	RenderEngine::Renderer::setDepthTest(true);

	{
		ResourceManager::setExecutablePath(argv[0]);
		Physics::PhysicsEngine::init();
		g_game->init();

		auto lastTime = std::chrono::high_resolution_clock::now();

		while (!glfwWindowShouldClose(pWindow))
		{
			auto currentTime = std::chrono::high_resolution_clock::now();
			double duration = std::chrono::duration<double, std::milli>(currentTime - lastTime).count();
			lastTime = currentTime;

			g_game->update(duration);

			Physics::PhysicsEngine::update(duration);

			RenderEngine::Renderer::clear();

			g_game->render();
			
			glfwSwapBuffers(pWindow);

			glfwPollEvents();
		}
		Physics::PhysicsEngine::terminate();
		g_game = nullptr;
		ResourceManager::unloadAllResources();
	}

	glfwTerminate();
	return 0;
}