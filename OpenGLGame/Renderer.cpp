#include "Renderer.h"

Renderer::Renderer() {
	Shader m_shader = Shader("Shaders\\Shader.vert", "Shaders\\Shader.frag");
}

void Renderer::update() {
	m_shader.bind();

	// m_cubeModel.bind(); https://github.com/Hopson97/HopsonCraft/blob/117dec4c00f337aff84dd33578d891c78729fcfd/Source/Renderer/SimpleRenderer.cpp

	for (auto& tile : m_tiles) {
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
}
/* What he did in this update function
	for (auto& cube : m_cubes)
	{
		prepare(*cube);
		glDrawElements(GL_TRIANGLES, m_cubeModel.getIndicesCount(), GL_UNSIGNED_INT, nullptr);
	}

	m_cubes.clear();
}

void SimpleRenderer::prepare(const Cube& cube)
{
	m_shader.setModelMatrix(createModelMatrix(cube));

}
*/


void Renderer::draw(const Tile& tile) {

}