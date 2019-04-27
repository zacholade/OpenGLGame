// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>
#include <chrono>
#include <vector>

#include "World.h"
#include "Renderer.h"


int main() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	std::cout << "Running GLFW Version: " << glfwGetVersionString() << std::endl;
	GLFWwindow* window = glfwCreateWindow(800, 800, "OpenGL", nullptr, nullptr); // glfwGetPrimaryMonitor() as 4th param for fullscreen
	glfwMakeContextCurrent(window);

	glViewport(0, 0, 800, 800);

	glewExperimental = GL_TRUE;
	glewInit();


	// Position(XY)   Color(RGB)    Texture(XY)
	// Verticies. (X, Y, R, G, B, X, Y)
	float vertices[] = {
	-0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Top-left
	0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Top-right
	0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // Bottom-right
	-0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f // Bottom-left
	};

	// Create a vbo
	GLuint vbo;
	glGenBuffers(1, &vbo); // Generate 1 buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	//Create a vao. Same way as a vbo.
	//Stores link between attributes and VBO with raw vertex data.
	//VAO doesn't store the vertex data itself, but instead a reference to my vbo I've created and how to retrieve attribute data from it.

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao); // Bind the vao to this program. Whenever glVertexAttribPointer is called, and this vao is bound, it will store that in this vao.\

	GLuint elements[] = {
		0, 1, 2,
		2, 3, 0
	};

	World world = World(10, 10);

	Renderer renderer = Renderer();

	while (!glfwWindowShouldClose(window)){
		glfwSwapBuffers(window);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwPollEvents();
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		renderer.update();

	}
	glfwTerminate();
}