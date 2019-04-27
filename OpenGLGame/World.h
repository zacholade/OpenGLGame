#pragma once

#include "Tile.h"
#include "Texture.h"

class World {
public:
	void makeNewWorld(int xSize, int ySize);

	Tile getBlockAt(int x, int y) const;
	static const int TILE_SIZE = 8;

private:
	std::vector<std::vector<Tile>> m_worldData;
};