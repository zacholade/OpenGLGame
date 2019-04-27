#pragma once

#include "Tile.h"
#include "Shader.h"

#include <vector>


class Renderer {
public:
	Renderer();

	void update();
	void draw(const Tile &tile);

private:
	std::vector<const Tile*> m_tiles;

	Shader m_shader;
};