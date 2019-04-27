#pragma once
#include <fstream>
#include <string>
#include <iostream>

#include "Tile.h"


class Shader {
public:
	Shader();
	Shader(const std::string& vertex_path, const std::string& fragment_path);
	GLuint loadShader(const std::string& vertex_path, const std::string& fragment_path);

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