#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <chrono>
#include <vector>

#include "Game/Game.h"
#include "Resources/ResourceManager.h"
#include "Renderer/Renderer.h"
#include "Physics/PhysicsEngine.h"

/**@file main.cpp*/

static constexpr unsigned int SCALE = 4; ///< Window scale amount.
static constexpr  unsigned int BLOCK_SIZE = 16; ///< A value indicating the total size of objects in the game.

glm::uvec2 g_windowSize(SCALE * 10 * BLOCK_SIZE, SCALE * 12 * BLOCK_SIZE); ///< Window Size.
std::unique_ptr<Game> g_game = std::make_unique<Game>(g_windowSize); ///< Indicates a class containing the main functions of the game.

/**
* @brief Function that tracks window size changes
* @param[out] pWindow Ptr, returning window dimensions
* @param[in] width Int width, accepts new width
* @param[in] height Int height, accepts new height
*/
void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height)
{
    g_windowSize.x = width;
    g_windowSize.y = height;
	g_game->setWindowSize(g_windowSize);
}

/**
* @brief Function showing which key is pressed
* @param[in] pWindow Ptr, shows which window the key was pressed on
* @parem[in] key Int, key that was pressed or released
* @param[in] scancode Int, the platform-specific scancode of the key
* @param[in] action Int, shows whether the button is pressed or not
* @param[in] mode Int, bit field describing which [modifier keys](mods) were held down 
*/
void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }

	g_game->setKey(key, action);
}

/**
* @brief Function showing which mouse button is pressed
* @param[in] pWindow Ptr, shows which window the key was pressed on
* @parem[in] button Int, button that was pressed or released
* @param[in] action Int, shows whether the button is pressed or not
* @param[in] mode Int, bit field describing which [modifier keys](mods) were held down
*/
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) 
{
	g_game->setButton(button, action);
}

/**
* @brief Function returning mouse position
* @param[in] pWindow Ptr, shows which window the key was pressed on
* @parem[in] xpos Double, X position
* @param[in] ypos Double, Y position
*/
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) 
{
	g_game->setMousePosition(xpos, ypos);
}


/**
* @brief Function with main game loop
* @parem[out] argc Int, The command line parameter, in this case shows the path to the files
* @param[out] argv Char**, Contains the names of the paths to the files
* @return zero if successfully
*/
int main(int argc, char** argv)
{
	if (!glfwInit()) ///< Initializing the main GLFW library
	{
		std::cout << "glfwInit failed" << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); ///< GLFW function. Specifying the GLFW version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* pWindow = glfwCreateWindow(g_windowSize.x, g_windowSize.y, "HMM", nullptr, nullptr); ///< Creating a main window using GLFW.
	if (!pWindow)
	{
		std::cout << "glfwCreateWindow failed" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwSetWindowSizeCallback(pWindow, glfwWindowSizeCallback); ///< GLFW function. Setting a callback to resize a window
	glfwSetKeyCallback(pWindow, glfwKeyCallback); ///< GLFW function. Setting a callback to track keys pressed on the keyboard

	glfwSetMouseButtonCallback(pWindow, mouse_button_callback); ///< GLFW function. Setting a callback to track keys pressed on the mouse
	glfwSetCursorPosCallback(pWindow, cursor_position_callback); ///< GLFW function. Setting a callback to track mouse cursor coordinates

	glfwMakeContextCurrent(pWindow); ///< GLFW function. Setting the current window

	if (!gladLoadGL()) ///< Initializing GLAD library
	{
		std::cout << "Can't load GLAD!" << std::endl;
	}

	std::cout << "Render: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

	RenderEngine::Renderer::setClearColor(0, 0, 0, 1); ///< Fill the screen with RGB color
	RenderEngine::Renderer::setDepthTest(true); ///< Function including color depth

	{
		ResourceManager::setExecutablePath(argv[0]); ///< Function that gets file paths
		Physics::PhysicsEngine::init(); ///< Function that initializes physics in the game
		g_game->init(); ///< Function that initializes the game

		auto lastTime = std::chrono::high_resolution_clock::now(); ///< Parameter counting time from frame

		while (!glfwWindowShouldClose(pWindow)) ///< Main game loop
		{
			auto currentTime = std::chrono::high_resolution_clock::now(); ///< Parameter counting the end of frame time
			double duration = std::chrono::duration<double, std::milli>(currentTime - lastTime).count();
			lastTime = currentTime;

			g_game->update(duration); ///< Update game state with duration frequency

			Physics::PhysicsEngine::update(duration); ///< Update the state of the game physics with frequency duration

			RenderEngine::Renderer::clear(); ///< Color clear game screen function

			g_game->render(); ///< Game rendering function
			
			glfwSwapBuffers(pWindow); ///< GLFW function. Sets the currently active window

			glfwPollEvents(); ///< GLFW function. Handles all expected events, such as window resizes or mouse clicks
		}
		Physics::PhysicsEngine::terminate(); ///< Cleaning up game physics
		g_game = nullptr; ///< Freeing memory from the game
		ResourceManager::unloadAllResources(); ///< Unloading all game resources
	}

	glfwTerminate(); ///< Destroying GLFW context
	return 0;
}