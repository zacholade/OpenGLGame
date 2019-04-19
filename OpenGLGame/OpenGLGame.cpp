#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include <string>
#include <fstream>
#include <chrono>

using namespace std;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr); // glfwGetPrimaryMonitor() as 4th param for fullscreen
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();

	/*
	Verticies. (X, Y, R, G, B)
	*/
	float vertices[] = {
	0.0f, 0.5f, 1.0f, 0.0f, 0.0f,
	0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
	-0.5f, -0.5f, 0.0f, 0.0f, 1.0f,	};


	// Create a vbo
	GLuint vbo;
	glGenBuffers(1, &vbo); // Generate 1 buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	/*
	Create a vao. Same way as a vbo.
	Stores link between attributes and VBO with raw vertex data.
	VAO doesn't store the vertex data itself, but instead a reference to my vbo I've created and how to retrieve attribute data from it.
	*/
	GLuint vao;
	glGenVertexArrays(1, &vao);

	std::string vertexShaderString =
		"#version 150\n"
		"in vec2 position;\n"
		"in vec3 color;\n"
		"out vec3 Color;\n"
		"void main(){\n"
		"Color = color;\n"
		"gl_Position = vec4(position, 0.0, 1.0);}";

	const char *vertexShaderSource = vertexShaderString.c_str();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLint vertexStatus;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexStatus);
	cout << "Vertex Shader Compile Status: " << vertexStatus << endl;

	std::string fragmentShaderString =
		"#version 150\n"
		"in vec3 Color;\n"
		"out vec4 outColor;\n"
		"void main(){\n"
		"outColor = vec4(Color, 1.0);}";

	const char* fragmentShaderSource = fragmentShaderString.c_str();

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	GLint fragmentStatus;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentStatus);
	cout << "Fragment Shader Compile Status: " << fragmentStatus << endl;
	
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glBindFragDataLocation(shaderProgram, 0, "outColor");

	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	glBindVertexArray(vao); // Bind the vao to this program. Whenever glVertexAttribPointer is called, and this vao is bound, it will store that in this vao.

	/*
	Discusses glVertexAttribPointer(args):

	The first parameter references the input. The second parameter specifies the
	number of values for that input, which is the same as the number of components
	of the vec. The third parameter specifies the type of each component and
	the fourth parameter specifies whether the input values should be normalized
	between -1.0 and 1.0 (or 0.0 and 1.0 depending on the format) if they aren’t
	floating point numbers.
	The last two parameters are arguably the most important here as they specify
	how the attribute is laid out in the vertex array. The first number specifies the
	stride, or how many bytes are between each position attribute in the array. The
	value 0 means that there is no data in between. This is currently the case as the
	position of each vertex is immediately followed by the position of the next vertex.
	The last parameter specifies the offset, or how many bytes from the start of the
	array the attribute occurs. Since there are no other attributes, this is 0 as well.
	*/

	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	cout << "Position Input of vertex shader: " << posAttrib << endl;

	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), 0); 	

	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));

	while (!glfwWindowShouldClose(window)){
		glfwSwapBuffers(window);
		glfwPollEvents();
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		glDrawArrays(GL_TRIANGLES, 0, 3);

	}
	glfwTerminate();
}

std::string readFile(const char* filePath) {
	std::string content;
	std::ifstream fileStream(filePath, std::ios::in);

	if (!fileStream.is_open()) {
		std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
		return "";
	}

	std::string line = "";
	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}