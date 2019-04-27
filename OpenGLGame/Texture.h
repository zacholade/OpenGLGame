#pragma once

#include <iostream>
#include <fstream>
#include <errno.h>
#include "GL/glew.h"

class Texture {
public:
	bool loadFromFile(const char* filePath);

	void bind() const;
	void unbind() const;

private:
	void clear();

	GLuint m_textureID;
};
