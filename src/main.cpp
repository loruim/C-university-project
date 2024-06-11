#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <chrono>
#include <vector>

#include "Game/Game.h"
#include "Resources/ResourceManager.h"
#include "Renderer/Renderer.h"

// glm::ivec2 g_windowSize(10*16+10*2, 5*16+10*2);
glm::ivec2 g_windowSize(8 * 16, 11 * 16);
std::unique_ptr<Game> g_game = std::make_unique<Game>(g_windowSize);

void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height)
{
    g_windowSize.x = width;
    g_windowSize.y = height;

	const float map_aspect_ratio = 7.f / 11.f;
	unsigned int viewPortWidth = g_windowSize.x;
	unsigned int viewPortHeight = g_windowSize.y;
	unsigned int viewPortLeftOffset = 0;
	unsigned int viewPortBottomOffset = 0;

	if (static_cast<float>(g_windowSize.x) / g_windowSize.y > map_aspect_ratio)
	{
		viewPortWidth = static_cast<unsigned int>(g_windowSize.y * map_aspect_ratio);
		viewPortLeftOffset = (g_windowSize.x - viewPortWidth) / 2;
	}
	else
	{
		viewPortHeight = static_cast<unsigned int>(g_windowSize.x / map_aspect_ratio);
		viewPortBottomOffset = (g_windowSize.y - viewPortHeight) / 2;
	}

	RenderEngine::Renderer::setViewport(viewPortWidth, viewPortHeight, viewPortLeftOffset, viewPortBottomOffset);
}

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }

	g_game->setKey(key, action);
}


int main(int argc, char** argv)
{
	if (!glfwInit())
	{
		std::cout << "glfwInit failed" << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // явное указание версии glfw
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* pWindow = glfwCreateWindow(g_windowSize.x, g_windowSize.y, "HMM", nullptr, nullptr);
	if (!pWindow)
	{
		std::cout << "glfwCreateWindow failed" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwSetWindowSizeCallback(pWindow, glfwWindowSizeCallback); // –аст€гивание окна
	glfwSetKeyCallback(pWindow, glfwKeyCallback);

	glfwMakeContextCurrent(pWindow); // ”становка текущего окна, так как окон может быть несколько

	if (!gladLoadGL()) // glad initialization
	{
		std::cout << "Can't load GLAD!" << std::endl;
	}

	std::cout << "Render: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

	RenderEngine::Renderer::setClearColor(0, 0, 0, 1);

	{
		ResourceManager::setExecutablePath(argv[0]);
		g_game->init();

		auto lastTime = std::chrono::high_resolution_clock::now();

		while (!glfwWindowShouldClose(pWindow))
		{
			auto currentTime = std::chrono::high_resolution_clock::now();
			uint64_t duretion = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - lastTime).count();
			lastTime = currentTime;

			g_game->update(duretion);

			RenderEngine::Renderer::clear();

			g_game->render();
			
			glfwSwapBuffers(pWindow);

			glfwPollEvents();
		}
		g_game = nullptr;
		ResourceManager::unloadAllResources();
	}

	glfwTerminate();
	return 0;
}