#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <chrono>
#include <vector>

#include "Renderer/ShaderProgram.h"
#include "Resources/ResourceManager.h"
#include "Renderer/Texture2D.h"
#include "Renderer/Sprite.h"
#include "Renderer/AnimatedSprite.h"

GLfloat point[] = {
     0.0f,  50.f, 0.0f,
     50.f, -50.f, 0.0f,
    -50.f, -50.f, 0.0f
};

GLfloat colors[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
};

GLfloat texCoord[] = {
	0.5f, 1.0f,
	1.0f, 0.0f,
	0.0f, 0.0f
};

glm::ivec2 g_windowSize(640, 480);

bool istank = false;

void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height)
{
    g_windowSize.x = width;
    g_windowSize.y = height;
    glViewport(0, 0, width, height);
}

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }

	if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
	{
		istank = !istank;
	}
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

	glClearColor(1, 1, 0, 1);

	{
		ResourceManager resourceManager(argv[0]);
		auto pDefaultShaderProgram = resourceManager.loadShaderProgram("DefaultShader", "res/shaders/vertex.txt", "res/shaders/fragment.txt");
		if (!pDefaultShaderProgram)
		{
			std::cerr << "Can't create shader program: " << "DefaultShader" << std::endl;
			return -1;
		}

		auto pSpriteShaderProgram = resourceManager.loadShaderProgram("SpriteShader", "res/shaders/vSprite.txt", "res/shaders/fSprite.txt");
		if (!pSpriteShaderProgram)
		{
			std::cerr << "Can't create sprite: " << "SpriteShader" << std::endl;
			return -1;
		}

		auto tex = resourceManager.loadTexture("DefaultTexture", "res/texture/battleCity.png");

		std::vector<std::string> subTexturesName = {
			"topYellowTankOne",
			"topYellowTankTwo",
			"leftYellowTankOne",
			"leftYellowTankTwo",
			"bottomYellowTankOne",
			"bottomYellowTankTwo",
			"RightYellowTankOne",
			"RightYellowTankTwo",
			"topWhiteTankOne",
			"topWhiteTankTwo",
			"leftWhiteTankOne",
			"leftWhiteTankTwo",
			"bottomWhiteTankOne",
			"bottomWhiteTankTwo",
			"RightWhiteTankOne",
			"RightWhiteTankTwo",
			"block",
			"rightBlock",
			"bottomBlock",
			"leftBlock",
			"topBlock" };

		auto pTexturesAtlas = resourceManager.loadTextureAtlas("DefaultTextureAtlas", "res/texture/battleCity.png", std::move(subTexturesName), 16, 16);

		auto pSprite = resourceManager.loadSprite("NewSprite", "DefaultTextureAtlas", "SpriteShader", 100, 100, "block");
		pSprite->setPosition(glm::vec2(300, 100));

		auto pAnimatedSprite = resourceManager.loadAnimatedSprite("NewAnimatedSprite", "DefaultTextureAtlas", "SpriteShader", 100, 100, "leftWhiteTankOne");
		
		pAnimatedSprite->setPosition(glm::vec2(300, 300));
		
		std::vector<std::pair<std::string, uint16_t>> blockState;
		blockState.emplace_back(std::make_pair<std::string, uint16_t>("block", 1000000000));
		blockState.emplace_back(std::make_pair<std::string, uint16_t>("rightBlock", 1000000000));
		blockState.emplace_back(std::make_pair<std::string, uint16_t>("bottomBlock", 1000000000));

		std::vector<std::pair<std::string, uint16_t>> tankState;
		tankState.emplace_back(std::make_pair<std::string, uint16_t>("topYellowTankOne", 1000000000));
		tankState.emplace_back(std::make_pair<std::string, uint16_t>("topYellowTankTwo", 1000000000));

		pAnimatedSprite->insertState("blockState", std::move(blockState));
		pAnimatedSprite->insertState("tankState", std::move(tankState));

		pAnimatedSprite->setState("blockState");

		GLuint point_vbo = 0;
		glGenBuffers(1, &point_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, point_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);

		GLuint color_vbo = 0;
		glGenBuffers(1, &color_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

		GLuint texCoord_vbo = 0;
		glGenBuffers(1, &texCoord_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, texCoord_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(texCoord), texCoord, GL_STATIC_DRAW);

		GLuint vao = 0;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, point_vbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, texCoord_vbo);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

		pDefaultShaderProgram->use();
		pDefaultShaderProgram->setInt("tex", 0);

		glm::mat4 modelMatrix_1 = glm::mat4(1.f);
		modelMatrix_1 = glm::translate(modelMatrix_1, glm::vec3(100.f, 50.f, 0.f));

		glm::mat4 modelMatrix_2 = glm::mat4(1.f);
		modelMatrix_2 = glm::translate(modelMatrix_2, glm::vec3(590.f, 50.f, 0.f));

		glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(g_windowSize.x), 0.f, static_cast<float>(g_windowSize.y), -100.f, 100.f);

		pDefaultShaderProgram->setMatrix4("projectionMat", projectionMatrix);

		pSpriteShaderProgram->use();
		pSpriteShaderProgram->setInt("tex", 0);
		pSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);

		auto lastTime = std::chrono::high_resolution_clock::now();

		while (!glfwWindowShouldClose(pWindow))
		{
			if (istank)
			{
				pAnimatedSprite->setState("tankState");
			}
			else 
			{
				pAnimatedSprite->setState("blockState");
			}

			auto currentTime = std::chrono::high_resolution_clock::now();
			uint16_t duretion = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - lastTime).count();
			lastTime = currentTime;

			pAnimatedSprite->update(duretion);

			glClear(GL_COLOR_BUFFER_BIT);

			pDefaultShaderProgram->use();
			glBindVertexArray(vao);
			tex->bind();

			pDefaultShaderProgram->setMatrix4("modelMat", modelMatrix_1);
			glDrawArrays(GL_TRIANGLES, 0, 3);

			pDefaultShaderProgram->setMatrix4("modelMat", modelMatrix_2);
			glDrawArrays(GL_TRIANGLES, 0, 3);

			pSprite->render();

			pAnimatedSprite->render();

			glfwSwapBuffers(pWindow);

			glfwPollEvents();
		}
	}

	glfwTerminate();
	return 0;
}