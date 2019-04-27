#pragma once

#include <fstream>
#include <GL/glew.h>


class Shader {
public:
	Shader(const char* vertex_path, const char* fragment_path);
	GLuint loadShader(const char* vertex_path, const char* fragment_path);

	void bind();
	void unbind();
	GLuint getID() const;

private:
	GLuint m_program;
	std::string readShaderFile(const GLchar* filePath);

	// TODO Abstract Shader class into ShaderProgram and Shader classes.
	// Different Shader class's inherit from shader program.
	// Allows for different programs with different configurations
};