#pragma once

#include "Texture.h"

#include <GL/glew.h> // TODO See if this is needed considering it is inside of Texture.h

class Tile {
private:
	int m_x, m_y;
	Texture m_texture;

public:
	Tile(int xPos, int yPos, Texture texture);

	int getX() const;
	int getY() const;

	std::vector<GLfloat> getTileVertices() const;
	std::vector<GLuint> getTileElements() const;
};

