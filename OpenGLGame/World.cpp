#include "World.h"


void World::makeNewWorld(int xSize, int ySize) {
	// for testing we load the texture here. Later on we need an atlas of sorts
	Texture texture;
	texture.loadFromFile("Textures\\Dirt.bmp");

	for (int x = 0; x < xSize; x++) {
		m_worldData.push_back(std::vector<Tile>());
		for (int y = 0; ySize < 10; y++) {
			Tile newTile = Tile(x, y, texture);
			m_worldData[x].push_back(newTile);
		}
	}
}
Tile World::getBlockAt(int x, int y) const {
	// TODO Ensure no index errors here
	return m_worldData[x][y];

}