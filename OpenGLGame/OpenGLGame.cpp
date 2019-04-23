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
#include <fstream>
#include <chrono>
#include <vector>
#include <filesystem>
#include <errno.h>


GLuint readBMPFile(const char* imagepath) {
	// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-5-a-textured-cube/

	FILE* file;
	errno_t err;

	// Data read from the header of the BMP file
	unsigned char header[54]; // Each BMP file begins by a 54-bytes header
	unsigned int dataPos;     // Position in the file where the actual data begins
	unsigned int width, height;
	unsigned int imageSize;   // = width*height*3
	// Actual RGB data
	unsigned char* data;

	if ((err = fopen_s(&file, imagepath, "rb")) != 0) {
		// https://stackoverflow.com/questions/28691612/how-to-go-from-fopen-to-fopen-s
		// File could not be opened. filepoint was set to NULL
		// error code is returned in err.
		// error message can be retrieved with strerror(err);
		std::cout << "Image could not be opened" << std::endl;
		return 0;
	}

	if (fread(header, 1, 54, file) != 54) { // If not 54 bytes read : problem
		std::cout << "Not a correct BMP file" << std::endl;
		return 0;
	}

	if (header[0] != 'B' || header[1] != 'M') {
		std::cout << "Not a correct BMP file" << std::endl;
		return 0;
	}

	// Read ints from the byte array according to their position in it
	dataPos = *(int*) & (header[10]);   // 0x0A
	imageSize = *(int*) & (header[34]); // 0x22
	width = *(int*) & (header[18]);     // 0x12
	height = *(int*) & (header[22]);    // 0x16

	// Some BMP files are misformatted, guess missing information
	if (imageSize == 0)    imageSize = width * height * 3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos == 0)      dataPos = 54; // The BMP header is done that way

	// Create a buffer
	data = new unsigned char[imageSize];

	// Read the actual data from the file into the buffer
	fread(data, 1, imageSize, file);

	//Everything is in memory now, the file can be closed
	fclose(file);

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	return textureID;
}


static std::string readShaderFile(const GLchar *filePath) {
	// https://badvertex.com/2012/11/20/how-to-load-a-glsl-shader-in-opengl-using-c.html
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


static GLuint loadShader(const char *vertex_path, const char *fragment_path) {
	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Read shaders
	std::string vertShaderStr = readShaderFile(vertex_path);
	std::string fragShaderStr = readShaderFile(fragment_path);
	const char* vertShaderSrc = vertShaderStr.c_str();
	const char* fragShaderSrc = fragShaderStr.c_str();

	GLint result = GL_FALSE;
	int logLength;

	// Compile vertex shader
	std::cout << "Compiling vertex shader." << std::endl;
	glShaderSource(vertShader, 1, &vertShaderSrc, NULL);
	glCompileShader(vertShader);

	// Check vertex shader
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &logLength);
	std::vector<char> vertShaderError((logLength > 1) ? logLength : 1);
	glGetShaderInfoLog(vertShader, logLength, NULL, &vertShaderError[0]);
	std::cout << &vertShaderError[0] << std::endl;

	// Compile fragment shader
	std::cout << "Compiling fragment shader." << std::endl;
	glShaderSource(fragShader, 1, &fragShaderSrc, NULL);
	glCompileShader(fragShader);

	// Check fragment shader
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &logLength);
	std::vector<char> fragShaderError((logLength > 1) ? logLength : 1);
	glGetShaderInfoLog(fragShader, logLength, NULL, &fragShaderError[0]);
	std::cout << &fragShaderError[0] << std::endl;

	std::cout << "Linking program" << std::endl;
	GLuint program = glCreateProgram();
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &result);
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
	std::vector<char> programError((logLength > 1) ? logLength : 1);
	glGetProgramInfoLog(program, logLength, NULL, &programError[0]);
	std::cout << &programError[0] << std::endl;

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	return program;
}


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

	glewExperimental = GL_TRUE;
	glewInit();

	/*
	Position(XY)   Color(RGB)    Texture(XY)
	Verticies. (X, Y, R, G, B, X, Y)
	*/
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

	/*
	Create a vao. Same way as a vbo.
	Stores link between attributes and VBO with raw vertex data.
	VAO doesn't store the vertex data itself, but instead a reference to my vbo I've created and how to retrieve attribute data from it.
	*/
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao); // Bind the vao to this program. Whenever glVertexAttribPointer is called, and this vao is bound, it will store that in this vao.\

	GLuint elements[] = {
		0, 1, 2,
		2, 3, 0
	};

	/*
	Create a ebo. Page 33
	*/
	GLuint ebo;
	glGenBuffers(1, &ebo);

	GLuint shaderProgram = loadShader("Shaders\\Shader.vert", "Shaders\\Shader.frag");
	glUseProgram(shaderProgram);

	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 7*sizeof(float), 0); 	

	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(2 * sizeof(float)));

	GLuint texture = readBMPFile("Textures\\Dirt.bmp");	GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");	glEnableVertexAttribArray(texAttrib);	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(5 * sizeof(float)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

	GLint uniTrans = glGetUniformLocation(shaderProgram, "trans");
	float z = 0;
	float x = 0;

	while (!glfwWindowShouldClose(window)){
		glfwSwapBuffers(window);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwPollEvents();
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			z = z - 0.001f;
		}

		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
			z = z + 0.001f;
		}

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			x = x - 0.001f;
		}

		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			x = x + 0.001f;
		}

		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::rotate(trans, z * glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		trans = glm::rotate(trans, x * glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(trans));

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	}
	glfwTerminate();
}