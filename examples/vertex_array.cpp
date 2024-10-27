#include <MKE/Window.hpp>
#include "MKE/Math/Vector.hpp"
#include "MKE/RenderWindow.hpp"
#include "MKE/VertexArray.hpp"

int main() {
	mk::RenderWindow window(800, 600, "VertexArray");

	std::array vertices = {
		// positions          // colors           // texture coords
		mk::Vertex2D({ 0.5f, 0.5f }, { 255, 0, 0 }, { 1.0f, 1.0f }),    // top right
		mk::Vertex2D({ 0.5f, -0.5f }, { 0, 255, 0 }, { 1.0f, 0.0f }),   // bottom right
		mk::Vertex2D({ -0.5f, -0.5f }, { 0, 0, 255 }, { 0.0f, 0.0f }),  // bottom left
		mk::Vertex2D({ -0.5f, 0.5f }, { 255, 255, 0 }, { 0.0f, 1.0f })  // top left
	};
	std::array indices = {
		// note that we start from 0!
		0u, 1u, 3u,  // first triangle
		1u, 2u, 3u   // second triangle
	};

	mk::VertexArray2D vertex_array(true);
	vertex_array.setSize(4);
	vertex_array.setVertexBuffer(vertices.begin(), vertices.size());
	vertex_array.setIndexBuffer(indices.begin(), indices.size());
	vertex_array.save();

	mk::Texture texture;
	texture.loadFromFile("arrow.png");
	texture.setSmooth(false);

	while (!window.isExitRequested()) {
		window.clear(mk::Colors::DARK);
		window.render(vertex_array, window.getCurrentView2D()->getTransform());
		window.display();
	}
}
