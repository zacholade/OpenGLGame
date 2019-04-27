#pragma once

#include <GL/glew.h>
#include <string>

class Texture {
public:
	bool loadFromFile(const std::string& filePath);

	void bind() const;
	void unbind() const;

private:
	void clear();

	GLuint m_textureID;
};
