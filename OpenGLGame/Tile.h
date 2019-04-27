#pragma once
#include <vector>

#include "Texture.h"


class Tile {
private:
	int m_x, m_y;
	Texture m_texture;

public:
	Tile(int xPos, int yPos, Texture texture);

	static const int TILE_SIZE = 8;

	int getX() const;
	int getY() const;

	std::vector<GLfloat> getTileVertices() const;
	static std::vector<GLuint> getTileElements();
};

