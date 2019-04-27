#include "Tile.h"

Tile::Tile(int xPos, int yPos, Texture texture) :
	m_x (xPos),
	m_y (yPos),
	m_texture (texture)
	{
}

int Tile::getX() const {
	return m_x;
}

int Tile::getY() const {
	return m_y;
}

std::vector<GLfloat> Tile::getTileVertices() const {
	return {
		(m_x - 1.0f) * TILE_SIZE, (m_x + 1.0f) * TILE_SIZE,  // Top Left
		(m_x + 1.0f) * TILE_SIZE, (m_x + 1.0f) * TILE_SIZE,   // Top Right
		(m_x + 1.0f) * TILE_SIZE, (m_x - 1.0f) * TILE_SIZE,   // Bottom Right
		(m_x - 1.0f) * TILE_SIZE, (m_x - 1.0f) * TILE_SIZE  // Bottom Left
	};
}

std::vector<GLuint> Tile::getTileElements() {
	return {
		0, 1, 2,      //  0 - 1        Two triangles per square. 
		2, 3, 0       //  - - -        They share two vertices. (0 and 2) Saves memory on the GPU
	};              //  3 - 2
}

